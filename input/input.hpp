#ifndef input_hpp
#define input_hpp

#include "../render/render.hpp"
#include "../declarations.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Input
{
public:
    void processInput(GLFWwindow* window);
    float deltaTime;
    float lastFrame;
private:
};
#endif /* input_hpp */