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
Scene c;
Audio a;
Audiobank b;
SignalEmitter e;

int main() 
{
    std::cout << "Initializing" << std::endl;
    if (s.initialize() == true)
    {
        r.initializeshaders();
        r.initializeskybox();
        r.initializelights();
        r.initializeSSAO();
        r.initializescenes();
        m.running = true;
        //this code needs to be HERE and NOT in s.initialize
        std::string vertexshadersource = g.readShader("/home/mizl/Documents/MGE/assets/shaders/main.vert");
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
    }
    while (m.running) 
    {
        if (glfwWindowShouldClose(s.window))
        {
            m.stop();
        }
        i.processInput(s.window);
        b.playaudiobank(b.audiobank);
        r.render(s.window);
    }
    return 0;
}