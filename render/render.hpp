#ifndef render_hpp
#define render_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../systems/systems.hpp"
#include "../shaders/shader.hpp"
#include "../textures/textures.hpp"
#include "../declarations.hpp"
#include <math.h>

class Render
{
public:
    void render(GLFWwindow* window);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)s.windoww / (float)s.windowh, 0.1f, 100.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    float yaw = -90.0f;
    float pitch = 0.0f;
private:
};

#endif /* render_hpp */