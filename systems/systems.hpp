#ifndef systems_hpp
#define systems_hpp

//include gl
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

/* NEVER ADD "#include ../render/render.hpp" here. biggest waste of 1 hour ever. */
#include "../shaders/shader.hpp"
#include "../declarations.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Systems
{
public:
    bool initialize();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    unsigned int VBO;
    unsigned int VAO;
    unsigned int lightVAO;
    unsigned int EBO;
    GLFWwindow* window;
    int windoww = 1280;
    int windowh = 720;
    float lastX = windoww/2, lastY = windowh/2;
    float sensitivity = 0.4f;
    float xoffset;
    float yoffset;
    bool firstMouse;
private:
};

#endif /* systems_hpp */