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
    float cameraSpeed = 3.0f * deltaTime; //adjust
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
}