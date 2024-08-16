#include "render.hpp"

Render::Render(){}

Render::~Render()
{
    delete mainshader;
    delete framebuffer;
}

void Render::initializeshaders()
{
    mainshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/main.vert", "/home/mizl/Documents/MGE/assets/shaders/main.frag");
    framebuffer = new Shader("/home/mizl/Documents/MGE/assets/shaders/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/fb.frag");
}

void Render::render(GLFWwindow* window)
{
    mainshader->setMat4("proj", proj);
    int once = 0;
    if (once = 0)
    {
        once++;
    }
    //calculate camera
    yaw   += s.xoffset;
    pitch += s.yoffset;

    s.xoffset *= 0.7;
    s.yoffset *= 0.7;

    if(pitch > 89.0f)
    {
        pitch =  89.0f;
    }
    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    cameraDirection.x = cos(glm::radians(yaw));
    cameraDirection.y = sin(glm::radians(pitch));  
    cameraDirection.z = sin(glm::radians(yaw));
    
    cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    cameraFront = glm::normalize(cameraDirection);

    //clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //use shaders
    mainshader->use();
    mainshader->setInt("material.Textureimg", 0);
    mainshader->setInt("material.specular", 1);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    //transformations
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    mainshader->setMat4("view", view);
    mainshader->setVec3("viewPos", cameraPos);

    mainshader->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    mainshader->setFloat("material.shininess", 32.0f);

    //lights starting here
    setPointLight(g, 0, 245, 86, 12, 69, glm::vec3(2.0f, 3.0f, 0.0f), 1.0f, 0.09f, 0.032f);
    setSpotLight(g, 1, 171, 188, 224, 100, cameraPos, cameraFront, glm::cos(glm::radians(17.5f)), glm::cos(glm::radians(25.0f)), 1.0f, 0.09f, 0.032f);
    //lights ending here

    //Model
    for (int i = 0; i < s.models.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i * 4.0f, 0.0f, -3.0f));
        float angle = 180.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        mainshader->setMat4("model", model);
        n.DrawModel(s.models.at(i));
    }
    //draw
    glfwSwapBuffers(window);
    glfwPollEvents();
    GLenum err;
}

void Render::setDirectionalLight(Shader g, int i, int re, int gr, int bl, int al, glm::vec3 direction, glm::vec3 specular)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    mainshader->setInt("light[" + std::to_string(i) + "].type", 0);
    mainshader->setVec3("light[" + std::to_string(i) + "].direction", direction);
    mainshader->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.025f , green * 0.025f, blue * 0.025f));
    mainshader->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    mainshader->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
}

void Render::setPointLight(Shader g, int i, int re, int gr, int bl, int al, glm::vec3 position, float constant, float linear, float quadratic)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    mainshader->setInt("light[" + std::to_string(i) + "].type", 1);
    mainshader->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.025f , green * 0.025f, blue * 0.025f));
    mainshader->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    mainshader->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
    mainshader->setVec3("light[" + std::to_string(i) + "].position", position);
    mainshader->setFloat("light[" + std::to_string(i) + "].constant", constant);
    mainshader->setFloat("light[" + std::to_string(i) + "].linear", linear);
    mainshader->setFloat("light[" + std::to_string(i) + "].quadratic", quadratic);
}

void Render::setSpotLight(Shader g, int i, int re, int gr, int bl, int al, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    mainshader->setInt("light[" + std::to_string(i) + "].type", 2);
    mainshader->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.025f , green * 0.025f, blue * 0.025f));
    mainshader->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    mainshader->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
    mainshader->setVec3("light[" + std::to_string(i) + "].position", position);
    mainshader->setVec3("light[" + std::to_string(i) + "].direction", direction);
    mainshader->setFloat("light[" + std::to_string(i) + "].cutOff", cutOff);
    mainshader->setFloat("light[" + std::to_string(i) + "].outerCutOff", outerCutOff);
    mainshader->setFloat("light[" + std::to_string(i) + "].constant", constant);
    mainshader->setFloat("light[" + std::to_string(i) + "].linear", linear);
    mainshader->setFloat("light[" + std::to_string(i) + "].quadratic", quadratic);
}