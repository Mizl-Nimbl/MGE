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
    for (int i = 0; i < scenes.size(); i++)
    {
        for (int j = 0; j < scenes[i].getLights().size(); j++)
        {
            lights.push_back(scenes[i].getLight(j));
        }
    }
}


void Render::initializeshaders()
{
    skyboxshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/skybox.vert", "/home/mizl/Documents/MGE/assets/shaders/skybox.frag");
    mainshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/main.vert", "/home/mizl/Documents/MGE/assets/shaders/shadowlighting.frag");
    textshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/text/text.vert", "/home/mizl/Documents/MGE/assets/shaders/text/text.frag");
    depthshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/depth.vert", "/home/mizl/Documents/MGE/assets/shaders/depth.frag");
    hdrshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/hdr.frag");
    blurshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/blur.frag");
    invertshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/invert.frag");
    greyshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/greyscale.frag");
    edgeshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/edgedetection.frag");
    edgehdrshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/edgehdr.frag");
    sharpenshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/sharpen.frag");
    posterizeshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/postprocessing/fb.vert", "/home/mizl/Documents/MGE/assets/shaders/postprocessing/posterize.frag");
    ssaoshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/ssao/ssao.vert", "/home/mizl/Documents/MGE/assets/shaders/ssao/ssao.frag");
    ssaoblurshader = new Shader("/home/mizl/Documents/MGE/assets/shaders/ssao/ssao.vert", "/home/mizl/Documents/MGE/assets/shaders/ssao/blurssao.frag");
}

void Render::initializeskybox()
{
    skyboxtex = t.initCubemap(faces);
}

void Render::initializeSSAO()
{
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, s.windoww, s.windowh, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, s.windoww, s.windowh, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            (rand() / (float)RAND_MAX) * 2.0 - 1.0,
            (rand() / (float)RAND_MAX) * 2.0 - 1.0,
            rand() / (float)RAND_MAX);
        sample = glm::normalize(sample);
        sample *= rand() / (float)RAND_MAX;
        float scale = float(i) / 64.0;
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            (rand() / (float)RAND_MAX) * 2.0 - 1.0,
            (rand() / (float)RAND_MAX) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, s.windoww, s.windowh, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, s.windoww, s.windowh, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Render::initializescenes()
{
    scenes = s.scenes;
}

void Render::initializeText()
{
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Render::renderScene(Shader* shader)
{
    for (int i = 0; i < scenes.size(); i++)
    {
        std::vector<Model> models = scenes.at(i).getModels(); // Store models in a local variable
        std::sort(models.begin(), models.end(), [this](Model a, Model b) {
            float distanceA = glm::length(this->cameraPos - a.getLocation());
            float distanceB = glm::length(this->cameraPos - b.getLocation());
            return distanceA > distanceB;
        });
        for (int j = 0; j < models.size(); j++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, models[j].getLocation());
            glm::vec3 rotation = models[j].getRotation();
            glm::vec3 scale = models[j].getScale();
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scale);
            shader->setMat4("model", model);
            models[j].DrawModel(models[j]);
        }
    }
}

/*
void Render::renderShadowMap()
{
    for (int i = 0; i < lights.size(); i++)
    {
        glCullFace(GL_FRONT);
        if (lights[i].getType() == 0 || lights[i].getType() == 2)
        {
            glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
            glm::mat4 lightView = glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + lights[i].getDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
            lightSpaceMatrix = lightProjection * lightView;
            depthshader->use();
            depthshader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, s.SHADOW_WIDTH, s.SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, s.depthmapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            renderScene(depthshader);
        }
        else if (lights[i].getType() == 1)
        {
            float aspect = (float)s.SHADOW_WIDTH / (float)s.SHADOW_HEIGHT;
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);

            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lights[i].getPosition(), lights[i].getPosition() + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0)));

            depthshader->use();
            for (unsigned int i = 0; i < 6; ++i)
            {
                depthshader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            }
            depthshader->setFloat("far_plane", 100.0f);
            depthshader->setVec3("lightPos", lights[i].getPosition());

            glViewport(0, 0, s.SHADOW_WIDTH, s.SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, s.depthmapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            renderScene(depthshader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);
    }
}
*/

void Render::renderSSAO()
{
    // 1. Generate SSAO texture
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoshader->use();
    for (unsigned int i = 0; i < 64; ++i)
    {
        ssaoshader->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    }
    ssaoshader->setMat4("projection", proj);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    renderScene(ssaoshader);

    // 2. Blur SSAO texture
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoblurshader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    renderScene(ssaoblurshader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::renderText(Font f)
{
    glm::vec3 color = f.getColor();
    std::string text = f.getText();
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;

    textshader->setVec3("textColor", color);

    glBindVertexArray(textVAO);
    glActiveTexture(GL_TEXTURE0);

    auto characters = f.getCharacters();
    for (char c : text) 
    {
        Character ch = characters[c];
        if (ch.textureID == 0)
        {
            std::cerr << "Error: textureID for character " << c << " is not valid." << std::endl;
        }
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] =
        {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::render(GLFWwindow* window)
{
    //scenes[0].setLightPosition(0, cameraPos);
    //renderShadowMap();
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

    s.xoffset *= 0.5 + 0.2;
    s.yoffset *= 0.5 + 0.2;

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
    mainshader->setInt("material.diffuse", 0);
    mainshader->setInt("material.specular", 1);
    mainshader->setInt("material.normal", 2);

    //transformations
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    mainshader->setMat4("view", view);
    mainshader->setVec3("viewPos", cameraPos);
    mainshader->setFloat("material.shininess", 32.0f);

    //lights starting here
    mainshader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    //glActiveTexture(GL_TEXTURE3);
    //glBindTexture(GL_TEXTURE_2D, s.depthmapFBO);
    mainshader->setInt("shadowMap", 3);
    //lights[0].setPosition(cameraPos);
    //lights[0].setDirection(cameraFront);
    for (int i = 0; i < lights.size(); i++)
    {
        std::string index = std::to_string(i);
        mainshader->setInt("light[" + index + "].type", lights[i].getType());
        mainshader->setVec3("light[" + index + "].position", lights[i].getPosition());
        mainshader->setVec3("light[" + index + "].direction", lights[i].getDirection());
        mainshader->setVec3("light[" + index + "].ambient", glm::vec3(lights[i].getRed() * 0.025f , lights[i].getGreen() * 0.025f, lights[i].getBlue() * 0.025f));
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
    //ssao pass

    renderSSAO();

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s.framebuffertex);
    hdrshader->setInt("scene", 0);
    hdrshader->setVec2("resolution", glm::vec2(s.windoww, s.windowh));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    hdrshader->setInt("ssao", 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    textshader->use();
    textshader->setMat4("projection", glm::ortho(0.0f, static_cast<float>(s.windoww), 0.0f, static_cast<float>(s.windowh)));
    for (int i = 0; i < scenes.size(); i++)
    {
        for (int j = 0; j < scenes.at(i).getTexts().size(); j++)
        {
            Font letters = scenes.at(i).getTexts().at(j);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(letters.getPosition().x, letters.getPosition().y, 0.0f));
            model = glm::rotate(model, glm::radians(letters.getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(letters.getScale(), letters.getScale(), 1.0f));
            textshader->setMat4("model", model);
            renderText(letters);
        }
    }

    //draw
    glfwSwapBuffers(window);
    glfwPollEvents();
    GLenum err;
}