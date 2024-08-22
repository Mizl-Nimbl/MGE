#include "input.hpp"
#include <iostream>

Input::Input()
{
    std::cout << "Input constructor called" << std::endl;
    std::filesystem::path path = "/home/mizl/Documents/MGE/saved/inputmap.xml";
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Failed to load XML file: " << path << std::endl;
    }
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) 
    {
        std::cerr << "No root element in XML file: " << path << std::endl;
    }
    tinyxml2::XMLElement* keyElement = root->FirstChildElement("KeyBindings");
    if (keyElement)
    {
        keyElement->QueryIntAttribute("forward", &forward);
        keyElement->QueryIntAttribute("backward", &backward);
        keyElement->QueryIntAttribute("left", &left);
        keyElement->QueryIntAttribute("right", &right);
        keyElement->QueryIntAttribute("jump", &jump);
        keyElement->QueryIntAttribute("crouch", &crouch);
        keyElement->QueryIntAttribute("sprint", &sprint);
        keyElement->QueryIntAttribute("quit", &quit);
        keyElement->QueryIntAttribute("uparrow", &uparrow);
        keyElement->QueryIntAttribute("downarrow", &downarrow);
        keyElement->QueryIntAttribute("leftarrow", &leftarrow);
        keyElement->QueryIntAttribute("rightarrow", &rightarrow);
    }
    std::cout << "Input map loaded" << std::endl;
}

void Input::processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame; 
    if (glfwGetKey(window, quit) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, sprint) == GLFW_PRESS)
    {
        cameraSpeed = 12.5f * deltaTime;
    }
    else
    {
        cameraSpeed = 6.5f * deltaTime;
    }
    if (glfwGetKey(window, forward) == GLFW_PRESS)
    {
        r.cameraPos += cameraSpeed * r.cameraFront;
    }
    if (glfwGetKey(window, backward) == GLFW_PRESS)
    {
        r.cameraPos -= cameraSpeed * r.cameraFront;
    }
    if (glfwGetKey(window, left) == GLFW_PRESS)
    {
        r.cameraPos -= glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, right) == GLFW_PRESS)
    {
        r.cameraPos += glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, jump) == GLFW_PRESS)
    {
        r.cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, crouch) == GLFW_PRESS)
    {
        r.cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, downarrow) == GLFW_PRESS)
    {
        e.emit(239498912);
    }
    if (glfwGetKey(window, uparrow) == GLFW_PRESS)
    {
        e.emit(909377451);
    }
    if (glfwGetKey(window, leftarrow) == GLFW_PRESS)
    {
        //placeholder
    }
    if (glfwGetKey(window, rightarrow) == GLFW_PRESS)
    {
        //placeholder
    }
}