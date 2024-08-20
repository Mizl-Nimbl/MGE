#ifndef render_hpp
#define render_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include <algorithm>

#include "../systems/systems.hpp"
#include "../shaders/shader.hpp"
#include "../textures/textures.hpp"
#include "../light/light.hpp"
#include "../declarations.hpp"

class Render
{
public:
    Render();
    ~Render();
    void initializeshaders();
    void initializeskybox();
    void initializelights();
    void initializeSSAO();
    void initializescenes();
    void renderScene(Shader* shader);
    void render(GLFWwindow* window);
    void renderShadowMap();
    void renderSSAO();
    std::vector<glm::vec3> ssaoKernel;
    std::vector<glm::vec3> ssaoNoise;
    unsigned int ssaoFBO, ssaoBlurFBO;
    unsigned int ssaoColorBuffer;
    unsigned int ssaoColorBufferBlur;
    unsigned int noiseTexture;
    unsigned int gPosition;
    unsigned int gNormal;
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

    std::vector <Light> lights;

    std::vector <Scene> scenes;

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
    Shader* edgehdrshader;
    Shader* sharpenshader;
    Shader* ssaoshader;
    Shader* ssaoblurshader;
};

#endif /* render_hpp */