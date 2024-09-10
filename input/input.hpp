#ifndef input_hpp
#define input_hpp

#include "../render/render.hpp"
#include "../audio/audio.hpp"
#include "../audiobank/audiobank.hpp"
#include "../signalemitter/signalemitter.hpp"
#include "../declarations.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyxml2.h>
#include <iostream>
#include <filesystem>

class Input
{
public:
    Input();
    void processInput(GLFWwindow* window);
private:
    float cameraSpeed;
    GLint forward;
    GLint backward;
    GLint left;
    GLint right;
    GLint jump;
    GLint crouch;
    GLint sprint;
    GLint quit;
    GLint uparrow;
    GLint downarrow;
    GLint leftarrow;
    GLint rightarrow;
};
#endif /* input_hpp */