#include "systems.hpp"
#include <iostream>

bool Systems::initialize()
{
    setenv("GLFW_PLATFORM", "wayland", 1);
    const char* glfwError;
    glfwSetErrorCallback([](int error, const char* description) 
    {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });
    if (!glfwInit()) 
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 720, "MGE", NULL, NULL);
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
    //framebuffer calls
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    unsigned int framebuffertex = t.initFramebuffer();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertex, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer Generated Successfully." << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    //use shaders
    //g.initshader("/home/mizl/Documents/MGE/assets/shaders/main.vert", "/home/mizl/Documents/MGE/assets/shaders/main.frag");
    //load models
    Model backpack("/home/mizl/Documents/MGE/assets/models/backpack/backpack.obj");
    Model test("/home/mizl/Documents/MGE/assets/models/test/test.obj");
    Model window("/home/mizl/Documents/MGE/assets/models/window/window.obj");
    models.push_back(backpack);
    models.push_back(test);
    models.push_back(window);
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
}