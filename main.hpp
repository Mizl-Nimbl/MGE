#ifndef main_hpp
#define main_hpp

#include "systems/systems.cpp"
#include "input/input.cpp"
#include "render/render.cpp"
#include "shaders/shader.cpp"
#include "textures/textures.cpp"
#include "mesh/mesh.cpp"
#include "model/model.cpp"
#include "light/light.cpp"
#include "scene/scene.cpp"
#include "audio/audio.cpp"
#include "audiobank/audiobank.cpp"
#include "signalemitter/signalemitter.cpp"
#include "font/font.cpp"
#include "physics/physics.cpp"
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
        glDeleteFramebuffers(1, &s.FBO);
        glDeleteFramebuffers(1, &s.depthmapFBO);
        glDeleteRenderbuffers(1, &s.RBO);
        glfwTerminate();
        running = false;
    }
private: 
};

#endif /* main_hpp */