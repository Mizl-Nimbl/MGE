#ifndef systems_hpp
#define systems_hpp

//include gl
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

/* NEVER ADD "#include ../render/render.hpp" here. biggest waste of 1 hour ever. */
#include "../scene/scene.hpp"
#include "../shaders/shader.hpp"
#include "../audio/audio.hpp"
#include "../audiobank/audiobank.hpp"
#include "../declarations.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>

class Systems
{
public:
    bool initialize();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    std::vector <Scene> getScenes();
    unsigned int VBO;
    unsigned int VAO;
    unsigned int lightVAO;
    unsigned int frameVAO;
    unsigned int frameVBO;
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
    unsigned int EBO;
    unsigned int FBO;
    unsigned int depthmapFBO;
    unsigned int RBO;
    GLFWwindow* window;
    int windoww = 1920;
    int windowh = 1080;
    int monitorcount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorcount);
    int SHADOW_WIDTH = 1024;
    int SHADOW_HEIGHT = 1024;
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 norotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 noscaling = glm::vec3(1.0f, 1.0f, 1.0f);
    float lastX = windoww/2, lastY = windowh/2;
    float sensitivity = 0.4f;
    float xoffset;
    float yoffset;
    bool firstMouse;
    std::vector <Scene> scenes;
    unsigned int framebuffertex;
    unsigned int initFramebuffer();
private:
};

#endif /* systems_hpp */