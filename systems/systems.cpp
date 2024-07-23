#include "systems.hpp"
#include <iostream>

bool Systems::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "MGE", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //size
    glViewport(0, 0, windoww, windowh);
    //callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //buffers
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);  
    //use shaders
    g.initshader("../shaders/main.vert", "../shaders/main.frag");
    return true;
}

void Systems::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Systems::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (s.firstMouse)
    {
        s.lastX = (float)xpos;
        s.lastY = (float)ypos;
        s.firstMouse = false;
    }

    s.xoffset = (float)xpos - s.lastX;
    s.yoffset = s.lastY - (float)ypos; // reversed since y-coordinates range from bottom to top

    s.lastX = (float)xpos;
    s.lastY = (float)ypos;
    
    s.xoffset *= s.sensitivity;
    s.yoffset *= s.sensitivity;

    std::cout << s.xoffset << std::endl;
}