#include <iostream>
#include "main.hpp"

Main m;
Systems s;
Input i;
Render r;
Shader g;
Textures t;

int main() 
{
    if (s.initialize() == true)
    {
        m.running = true;
        //this code needs to be HERE and NOT in s.initialize
        std::string vertexshadersource = g.readShader("shaders/vertex.vert");
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        g.setMat4("proj", r.proj);
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