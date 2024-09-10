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
    if (glfwGetKey(window, quit) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, sprint) == GLFW_PRESS)
    {
        //cameraSpeed = 12.5f * deltaTime;
        e.emit(70);
    }
    else
    {
        //cameraSpeed = 6.5f * deltaTime;
    }
    if (glfwGetKey(window, forward) == GLFW_PRESS)
    {
        //r.cameraPos += cameraSpeed * r.cameraFront;
        e.emit(10);
    }
    if (glfwGetKey(window, backward) == GLFW_PRESS)
    {
        //r.cameraPos -= cameraSpeed * r.cameraFront;
        e.emit(20);
    }
    if (glfwGetKey(window, left) == GLFW_PRESS)
    {
        //r.cameraPos -= glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
        e.emit(30);
    }
    if (glfwGetKey(window, right) == GLFW_PRESS)
    {
        //r.cameraPos += glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * cameraSpeed;
        e.emit(40);
    }
    if (glfwGetKey(window, jump) == GLFW_PRESS)
    {
        //r.cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        e.emit(50);
    }
    if (glfwGetKey(window, crouch) == GLFW_PRESS)
    {
        //r.cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        e.emit(60);
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