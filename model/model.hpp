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
#include "../systems/systems.hpp"
#include "../declarations.hpp"

class Model
{
public:
    Model() = default;
    Model(std::string const &path)
    {
        loadModel(path);
        makeModelBox();
    }
    void DrawModel(Model model)
    {
        model.Draw();
    }
    void Draw();
    void setLocation(glm::vec3 newLocation);
    void setLastLocation(glm::vec3 newLocation);
    void setLastVelocity(glm::vec3 newVelocity);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    glm::vec3 getLocation();
    glm::vec3 getLastLocation();
    glm::vec3 getLastVelocity();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    std::vector<glm::vec3> getBox();
    Mesh getMesh(int i);
    std::vector<Mesh> getMeshes();
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::vector<glm::vec3> modelBox;
    std::string directory;
    glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lastLocation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lastVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    bool compareVec3Min(glm::vec3 a, glm::vec3 b);
    bool compareVec3Max(glm::vec3 a, glm::vec3 b);
    void loadModel(std::string const &path);
    void makeModelBox();
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char* typeName, const std::string& area);
};

#endif /* model_hpp */