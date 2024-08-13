#ifndef model_hpp
#define model_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/stb/stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../shaders/shader.hpp"
#include "../textures/textures.hpp"
#include "../declarations.hpp"

class Model
{
public:
    void initModel(std::string const &path)
    {
        loadModel(path);
    }
    void Draw();
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    const char* directory;
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char* typeName);
    bool isTextureLoaded(const std::vector<Texture>& textures_loaded, const aiString& str, Texture& outTexture);
};

#endif /* model_hpp */