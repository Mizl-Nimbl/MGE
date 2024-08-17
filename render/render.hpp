#ifndef render_hpp
#define render_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>

#include "../systems/systems.hpp"
#include "../shaders/shader.hpp"
#include "../textures/textures.hpp"
#include "../declarations.hpp"

class Render
{
public:
    Render();
    ~Render();
    void initializeshaders();
    void initializeskybox();
    void renderScene(Shader* shader);
    void render(GLFWwindow* window);
    void renderShadowMap();
    void setDirectionalLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 direction);
    void setPointLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 position, float constant, float linear, float quadratic);
    void setSpotLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)s.windoww / (float)s.windowh, 0.1f, 100.0f);
    glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    //use skybox
    std::vector<std::string> faces = 
    {
        "/home/mizl/Documents/MGE/assets/images/skybox2/right.jpg",
        "/home/mizl/Documents/MGE/assets/images/skybox2/left.jpg",
        "/home/mizl/Documents/MGE/assets/images/skybox2/top.jpg",
        "/home/mizl/Documents/MGE/assets/images/skybox2/bottom.jpg",
        "/home/mizl/Documents/MGE/assets/images/skybox2/front.jpg",
        "/home/mizl/Documents/MGE/assets/images/skybox2/back.jpg"
    };
    unsigned int skyboxtex;

    float yaw = -90.0f;
    float pitch = 0.0f;
private:
    Shader* skyboxshader;
    Shader* mainshader;
    Shader* depthshader;
    Shader* hdrshader;
    Shader* blurshader;
    Shader* invertshader; 
    Shader* greyshader;
    Shader* edgeshader; 
    Shader* sharpenshader;
};

#endif /* render_hpp */