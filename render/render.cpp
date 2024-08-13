#include "render.hpp"

void Render::render(GLFWwindow* window)
{
    int once = 0;
    if (once = 0)
    {
        once++;
    }
    //calculate camera
    yaw   += s.xoffset;
    pitch += s.yoffset;

    s.xoffset *= 0.5;
    s.yoffset *= 0.5;

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
    g.use();
    g.setInt("material.Textureimg", 0);
    g.setInt("material.specular", 1);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    //transformations
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    g.setMat4("view", view);
    g.setVec3("viewPos", cameraPos);

    g.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    g.setFloat("material.shininess", 32.0f);

    //lights starting here
    g.setInt("light[0].type", 1);
    g.setVec3("light[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    g.setVec3("light[0].diffuse", glm::vec3(0.5f, 0.1f, 0.1f));
    g.setVec3("light[0].specular", glm::vec3(0.5f, 0.05f, 0.05f));
    g.setVec3("light[0].position", glm::vec3(1.0f, 1.0f, 10.0f));
    g.setFloat("light[0].constant",  1.0f);
    g.setFloat("light[0].linear",    0.09f);
    g.setFloat("light[0].quadratic", 0.032f);

    g.setInt("light[1].type", 1);
    g.setVec3("light[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    g.setVec3("light[1].diffuse", glm::vec3(0.1f, 0.1f, 0.5f));
    g.setVec3("light[1].specular", glm::vec3(0.3f, 0.3f, 0.9f));
    g.setVec3("light[1].position", glm::vec3(-1.0f, -1.0f, -7.0f));
    g.setFloat("light[1].constant",  1.0f);
    g.setFloat("light[1].linear",    0.09f);
    g.setFloat("light[1].quadratic", 0.032f);
    //lights ending here
    
    /*
    unsigned int texture0 = t.initTexture("/home/mizl/Documents/MGE/assets/images/container2.png");
    unsigned int texture1 = t.initTexture("/home/mizl/Documents/MGE/assets/images/container2_specular.png");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    */

    for (int i = 0; i < 1; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        g.setMat4("model", model);
        n.Draw();
    }
    //draw
    glfwSwapBuffers(window);
    glfwPollEvents();
    GLenum err;
}