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
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //framebuffer calls
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    framebuffertex = initFramebuffer();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer Generated Successfully." << std::endl;
    }
    //depth framebuffer
    glGenFramebuffers(1, &depthmapFBO);
    SHADOW_WIDTH = 1024;
    SHADOW_HEIGHT = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //rebind original buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //frameVAO
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &frameVAO);
    glGenBuffers(1, &frameVBO);
    glBindVertexArray(frameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    //skybox
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    //load models
    //Model test("/home/mizl/Documents/MGE/assets/models/test/test.obj");
    //test.setLocation(glm::vec3(0.0f, 0.0f, 0.0f));
    //models.push_back(test);
    //Model skull("/home/mizl/Documents/MGE/assets/models/skull/skull.obj");
    //skull.setLocation(glm::vec3(3.0f, 0.0f, 0.0f));
    //models.push_back(skull);
    //Model brain("/home/mizl/Documents/MGE/assets/models/brain/brain.obj");
    //brain.setLocation(glm::vec3(3.0f, 0.0f, 0.0f));
    //models.push_back(brain);
    //Model window("/home/mizl/Documents/MGE/assets/models/window/window.obj");
    //window.setLocation(glm::vec3(0.0f, 3.0f, 0.0f));
    //models.push_back(window);

    //glm::vec3 loc1 = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 rot1 = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 sca1 = glm::vec3(1.0f, 1.0f, 1.0f);
    //Scene mainscene(loc1, rot1, sca1);
    //mainscene.setModel("/home/mizl/Documents/MGE/assets/models/test/test.obj");
    //mainscene.setModel("/home/mizl/Documents/MGE/assets/models/skull/skull.obj");
    //mainscene.setModel("/home/mizl/Documents/MGE/assets/models/brain/brain.obj");
    //mainscene.setModel("/home/mizl/Documents/MGE/assets/models/window/window.obj");
    //mainscene.setGlobalLocation(glm::vec3(0.0f, -3.0f, 0.0f));
    //mainscene.setModelLocation(0, glm::vec3(0.0f, 0.0f, 0.0f));
    //mainscene.setModelLocation(1, glm::vec3(3.0f, 0.0f, 0.0f));
    //mainscene.setModelLocation(2, glm::vec3(3.0f, 0.0f, 0.0f));
    //mainscene.setModelLocation(3, glm::vec3(0.0f, 3.0f, 0.0f));
    //scenes.push_back(mainscene);

    scenes = getScenes();

    return true;
}

std::vector<Scene> Systems::getScenes()
{
    std::vector<Scene> scenes;
    std::string scenesFolderPath = "/home/mizl/Documents/MGE/assets/scenes";

    for (const auto& entry : std::filesystem::directory_iterator(scenesFolderPath))
    {
        if (entry.path().extension() == ".xml")
        {
            Scene scene(entry.path().string());
            scenes.push_back(scene);
        }
    }
    return scenes;
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

unsigned int Systems::initFramebuffer()
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    return texture;
}