#include "render.hpp"

Render::Render(){}

Render::~Render()
{
    delete mainshader;
    delete hdrshader;
    delete skyboxshader;
    delete depthshader;
    delete blurshader;
    delete invertshader;
    delete greyshader;
    delete edgeshader;
    delete sharpenshader;
}

void Render::initializelights()
{
    Light pointlight(245, 86, 12, 100, glm::vec3(2.0f, 3.0f, 0.0f), 1.0f);
    Light flashlight(171, 188, 224, 69, cameraPos, cameraFront, 1.0f);
    lights.push_back(pointlight);
    lights.push_back(flashlight);
}

void Render::initializeshaders()
{
    skyboxshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/skybox.vert", "/home/mizl/Documents/MGE/assets/shaders/skybox.frag");
    mainshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/main.vert", "/home/mizl/Documents/MGE/assets/shaders/main.frag");
    depthshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/depth.vert", "/home/mizl/Documents/MGE/assets/shaders/depth.frag");
    hdrshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/hdr.frag");
    blurshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/blur.frag");
    invertshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/invert.frag");
    greyshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/greyscale.frag");
    edgeshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/edgedetection.frag");
    sharpenshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/sharpen.frag");
}

void Render::initializeskybox()
{
    skyboxtex = t.initCubemap(faces);
}

void Render::renderScene(Shader* shader)
{
    for (int i = 0; i < s.models.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, s.models.at(i).getLocation());
        glm::vec3 rotation = s.models.at(i).getRotation();
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        shader->setMat4("model", model);
        n.DrawModel(s.models.at(i));
    }
}

void Render::renderShadowMap()
{
    glCullFace(GL_FRONT);
    float near_plane = 0.1f;
    float far_plane = 10.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    //glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 10.0f, -1.0f), glm::vec3(0.0f, 0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f));
    glm::mat4 lightView = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
    lightSpaceMatrix = lightProjection * lightView;
    depthshader->use();
    depthshader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, s.SHADOW_WIDTH, s.SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, s.depthmapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    renderScene(depthshader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
}

void Render::render(GLFWwindow* window)
{
    renderShadowMap();
    glViewport(0, 0, s.windoww, s.windowh);
    //first pass
    glBindFramebuffer(GL_FRAMEBUFFER, s.FBO);
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);
    
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

    view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    skyboxshader->use();
    skyboxshader->setInt("skybox", 0);
    skyboxshader->setMat4("proj", proj);
    skyboxshader->setMat4("view", view);
    glBindVertexArray(s.skyboxVAO);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxtex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    //use shaders
    mainshader->use();
    mainshader->setMat4("proj", proj);
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
    //setDirectionalLight(mainshader, 2, 200, 200, 200, 40, glm::vec3(-2.0f, 10.0f, -1.0f));
    //mainShader->setVec3("lightPos", lightPos);
    mainshader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    //setPointLight(mainshader, 1, 245, 86, 12, 100, glm::vec3(2.0f, 3.0f, 0.0f), 1.0f, 0.09f, 0.032f);
    //setSpotLight(mainshader, 0, 171, 188, 224, 69, cameraPos, cameraFront, glm::cos(glm::radians(17.5f)), glm::cos(glm::radians(25.0f)), 1.0f, 0.09f, 0.032f);
    lights[1].setPosition(cameraPos);
    lights[1].setDirection(cameraFront);
    for (int i = 0; i < lights.size(); i++) 
    {
        std::string index = std::to_string(i);
        mainshader->setInt("light[" + index + "].type", lights[i].getType());
        mainshader->setVec3("light[" + index + "].position", lights[i].getPosition());
        mainshader->setVec3("light[" + index + "].direction", lights[i].getDirection());
        mainshader->setVec3("light[" + index + "].ambient", glm::vec3(lights[i].getRed() * 0.009f , lights[i].getGreen() * 0.009f, lights[i].getBlue() * 0.009f));
        mainshader->setVec3("light[" + index + "].diffuse", glm::vec3(lights[i].getRed(), lights[i].getGreen(), lights[i].getBlue()));
        mainshader->setVec3("light[" + index + "].specular", glm::vec3(lights[i].getRed(), lights[i].getGreen(), lights[i].getBlue()));
        mainshader->setFloat("light[" + index + "].cutOff", lights[i].getCutOff());
        mainshader->setFloat("light[" + index + "].outerCutOff", lights[i].getOuterCutOff());
        mainshader->setFloat("light[" + index + "].constant", lights[i].getConstant());
        mainshader->setFloat("light[" + index + "].linear", lights[i].getLinear());
        mainshader->setFloat("light[" + index + "].quadratic", lights[i].getQuadratic());
    }
    //lights ending here

    renderScene(mainshader);
    //second pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    hdrshader->use();
    //blurshader->use();
    //invertshader->use();
    //greyshader->use();
    //edgeshader->use();
    //sharpenshader->use();
    glBindVertexArray(s.frameVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, s.framebuffertex);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //draw
    glfwSwapBuffers(window);
    glfwPollEvents();
    GLenum err;
}

/*
void Render::setDirectionalLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 direction)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    g->setInt("light[" + std::to_string(i) + "].type", 0);
    g->setVec3("light[" + std::to_string(i) + "].direction", direction);
    g->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.009f , green * 0.009f, blue * 0.009f));
    g->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    g->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
}

void Render::setPointLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 position, float constant, float linear, float quadratic)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    g->setInt("light[" + std::to_string(i) + "].type", 1);
    g->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.009f , green * 0.009f, blue * 0.009f));
    g->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    g->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
    g->setVec3("light[" + std::to_string(i) + "].position", position);
    g->setFloat("light[" + std::to_string(i) + "].constant", constant);
    g->setFloat("light[" + std::to_string(i) + "].linear", linear);
    g->setFloat("light[" + std::to_string(i) + "].quadratic", quadratic);
}

void Render::setSpotLight(Shader* g, int i, int re, int gr, int bl, int al, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
    float bright = (al / 255.0f);
    float red    = (re / 255.0f) * bright;
    float green  = (gr / 255.0f) * bright;
    float blue   = (bl / 255.0f) * bright;
    g->setInt("light[" + std::to_string(i) + "].type", 2);
    g->setVec3("light[" + std::to_string(i) + "].ambient", glm::vec3(red * 0.009f , green * 0.009f, blue * 0.009f));
    g->setVec3("light[" + std::to_string(i) + "].diffuse", glm::vec3(red, green, blue));
    g->setVec3("light[" + std::to_string(i) + "].specular", glm::vec3(red, green, blue));
    g->setVec3("light[" + std::to_string(i) + "].position", position);
    g->setVec3("light[" + std::to_string(i) + "].direction", direction);
    g->setFloat("light[" + std::to_string(i) + "].cutOff", cutOff);
    g->setFloat("light[" + std::to_string(i) + "].outerCutOff", outerCutOff);
    g->setFloat("light[" + std::to_string(i) + "].constant", constant);
    g->setFloat("light[" + std::to_string(i) + "].linear", linear);
    g->setFloat("light[" + std::to_string(i) + "].quadratic", quadratic);
}
*/