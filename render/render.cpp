#include "render.hpp"

void Render::render(GLFWwindow* window)
{
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

    //data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3
    };

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    //transformations
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    //send to buffers and shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    g.setMat4("view", view);
    g.setVec3("viewPos", cameraPos);

    g.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    g.setFloat("material.shininess", 32.0f);

    g.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    g.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    g.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    g.setVec3("light.position", glm::vec3(1.0f, 1.0f, 1.0f));

    g.setFloat("light.constant",  1.0f);
    g.setFloat("light.linear",    0.09f);
    g.setFloat("light.quadratic", 0.032f);	

    //bind
    glBindVertexArray(s.lightVAO);
    glBindVertexArray(s.VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.EBO);
    
    glActiveTexture(GL_TEXTURE0);
    unsigned int texture0 = t.initTexture("../textures/container2.png");
    glBindTexture(GL_TEXTURE_2D, texture0);

    
    glActiveTexture(GL_TEXTURE1);
    unsigned int texture1 = t.initTexture("../textures/container2_specular.png");
    glBindTexture(GL_TEXTURE_2D, texture1);

    //draw
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,  0.0f,  0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    g.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glfwSwapBuffers(window);
    glfwPollEvents();
}