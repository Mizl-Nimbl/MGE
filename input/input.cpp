#include "input.hpp"
#include <iostream>

void Input::processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame; 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 6.5f * deltaTime; //adjust
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        r.cameraPos += cameraSpeed * r.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        r.cameraPos -= cameraSpeed * r.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        r.cameraPos -= glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        r.cameraPos += glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        r.cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        r.cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        b.audiobank[0]->queue();
    }
}