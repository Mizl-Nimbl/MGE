#include <iostream>
#include "main.hpp"

Main m;
Systems s;
Input i;
Render r;
Shader g;
Textures t;
Mesh h;
Model n;
Light l;

int main() 
{
    if (s.initialize() == true)
    {
        r.initializeshaders();
        r.initializeskybox();
        r.initializelights();
        r.initializeSSAO();
        m.running = true;
        //this code needs to be HERE and NOT in s.initialize
        std::string vertexshadersource = g.readShader("/home/mizl/Documents/MGE/assets/shaders/main.vert");
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        
    }
    std::cout << "Initializing" << std::endl;
    while (m.running) 
    {
        if (glfwWindowShouldClose(s.window))
        {
            m.stop();
        }
        i.processInput(s.window);
        r.render(s.window);
    }
    return 0;
}