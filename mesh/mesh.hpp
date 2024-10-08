#ifndef mesh_hpp
#define mesh_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "../shaders/shader.hpp"
#include "../declarations.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    int boneIDs;
    float boneWeights;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
    std::string area;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(std::vector<Texture> meshtextures);
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    std::vector<Texture> textures_loaded;
};

#endif /* mesh_hpp */