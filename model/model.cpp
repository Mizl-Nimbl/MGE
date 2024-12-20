#include "model.hpp"

void Model::Draw()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(textures_loaded);
    }
} 

void Model::setLocation(glm::vec3 newLocation) 
{
    location = newLocation;
}

void Model::setLastLocation(glm::vec3 newLocation) 
{
    lastLocation = newLocation;
}

void Model::setLastVelocity(glm::vec3 newVelocity) 
{
    lastVelocity = newVelocity;
}

void Model::setRotation(glm::vec3 newRotation) 
{
    rotation = newRotation;
}

void Model::setScale(glm::vec3 newScale) 
{
    scale = newScale;
}

glm::vec3 Model::getLocation()
{
    return location;
}

glm::vec3 Model::getLastLocation()
{
    return lastLocation;
}

glm::vec3 Model::getLastVelocity()
{
    return lastVelocity;
}

glm::vec3 Model::getRotation()
{
    return rotation;
}

glm::vec3 Model::getScale()
{
    return scale;
}

void Model::loadModel(std::string const &path)
{
    meshes.clear();
    textures_loaded.clear();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals); 

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    std::string directoryStr = path.substr(0, path.find_last_of('/'));
    directory = directoryStr.c_str();

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        // Tangents
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;

        // Bitangents
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector;

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        unsigned int idx0 = face.mIndices[0];
        unsigned int idx1 = face.mIndices[1];
        unsigned int idx2 = face.mIndices[2];

        Vertex& v0 = vertices[idx0];
        Vertex& v1 = vertices[idx1];
        Vertex& v2 = vertices[idx2];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec2 deltaUV1 = v1.texCoords - v0.texCoords;
        glm::vec2 deltaUV2 = v2.texCoords - v0.texCoords;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent, bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        v0.tangent += tangent;
        v1.tangent += tangent;
        v2.tangent += tangent;

        v0.bitangent += bitangent;
        v1.bitangent += bitangent;
        v2.bitangent += bitangent;
    }
    
    // Normalize tangents and bitangents
    
    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i].tangent = glm::normalize(vertices[i].tangent);
        vertices[i].bitangent = glm::normalize(vertices[i].bitangent);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", "normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::cout << "Loaded " << diffuseMaps.size() << " diffuse textures and " << specularMaps.size() << " specular textures and " << normalMaps.size() << " normal maps for mesh." << std::endl;
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char* typeName, const std::string& area)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        Texture texture;
        mat->GetTexture(type, i, &str);
        bool textureLoaded = false;
        if (textures_loaded.size() > i) {
            textures.push_back(texture);
            //std::cout << "Texture already loaded" << std::endl;
            textureLoaded = true;
            continue;
        }

        // if texture hasn't been loaded already, load it
        std::string fullPath = std::string(directory) + '/' + area + ".png";
        std::cout << "Loading texture from path: " << fullPath << std::endl;
        // Check if the file exists before calling initTexture
        std::ifstream file(fullPath);
        if (!file.good()) {
            std::cerr << "Error: Texture file not found at path: " << fullPath << std::endl;
            continue;
        }
        texture.id = t.initTexture(fullPath.c_str());
        if (texture.id == 0) {
            std::cerr << "Error: Failed to initialize texture from path: " << fullPath << std::endl;
            continue;
        }
        texture.type = typeName;
        texture.path = fullPath;
        texture.area = area;
        textures.push_back(texture);
        textures_loaded.push_back(texture); // add to loaded textures
    }
    return textures;
}

Mesh Model::getMesh(int i)
{
    return meshes[i];
}

std::vector<Mesh> Model::getMeshes()
{
    return meshes;
}

void Model::makeModelBox()
{
    std::vector<glm::vec3> modelArr;
    for (int i = 0; i < meshes.size(); i++)
    {
        
        for (int j = 0; j < meshes[i].vertices.size(); j++)
        {
            modelArr.push_back(meshes[i].vertices[j].position);
        }
    }
    glm::vec3 bottomleftback = *std::min_element(modelArr.begin(), modelArr.end(), 
        [this](const glm::vec3& a, const glm::vec3& b) { return compareVec3Min(a, b); });
    glm::vec3 toprightfront = *std::max_element(modelArr.begin(), modelArr.end(), 
        [this](const glm::vec3& a, const glm::vec3& b) { return compareVec3Max(a, b); });
    glm::vec3 bottomrightback = glm::vec3(toprightfront.x, bottomleftback.y, bottomleftback.z);
    glm::vec3 bottomleftfront = glm::vec3(bottomleftback.x, bottomleftback.y, toprightfront.z);
    glm::vec3 bottomrightfront = glm::vec3(toprightfront.x, bottomleftback.y, toprightfront.z);
    glm::vec3 topleftback = glm::vec3(bottomleftback.x, toprightfront.y, bottomleftback.z);
    glm::vec3 topleftfront = glm::vec3(bottomleftback.x, toprightfront.y, toprightfront.z);
    glm::vec3 toprightback = glm::vec3(toprightfront.x, toprightfront.y, bottomleftback.z);
    modelBox.push_back(bottomleftback + location);
    modelBox.push_back(toprightfront + location);
    modelBox.push_back(bottomrightback + location);
    modelBox.push_back(bottomleftfront + location);
    modelBox.push_back(bottomrightfront + location);
    modelBox.push_back(topleftback + location);
    modelBox.push_back(topleftfront + location);
    modelBox.push_back(toprightback + location);
}

bool Model::compareVec3Min(glm::vec3 a, glm::vec3 b) {
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y < b.y;
    return a.z < b.z;
}

bool Model::compareVec3Max(glm::vec3 a, glm::vec3 b) {
    if (a.x != b.x) return a.x > b.x;
    if (a.y != b.y) return a.y > b.y;
    return a.z > b.z;
}

std::vector<glm::vec3> Model::getBox()
{
    return modelBox;
}