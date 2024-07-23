#ifndef main_hpp
#define main_hpp

#include "systems/systems.cpp"
#include "input/input.cpp"
#include "render/render.cpp"
#include "shaders/shader.cpp"
#include "textures/textures.cpp"
#include "declarations.hpp"

class Main 
{
public:
    bool running;
    void stop()
    {
        glDeleteVertexArrays(1, &s.VAO);
        glDeleteBuffers(1, &s.VBO);
        glDeleteBuffers(1, &s.EBO);
        glfwTerminate();
        running = false;
    }
private: 
};

#endif /* main_hpp */