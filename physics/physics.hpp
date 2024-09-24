#ifndef physics_hpp
#define physics_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include <algorithm>

#include "../scene/scene.hpp"
#include "../signalemitter/signalemitter.hpp"
#include "../render/render.hpp"
#include "../declarations.hpp"

class Physics
{
public:
    Physics();
    void ProcessPhysics(std::vector<Scene> scenes);
    void Movement();

    //getters
    glm::vec3 getObjPos(Model m);
    glm::vec3 getObjVel(Model m);
    glm::vec3 getObjAcc(Model m);

    //setters
    void setObjPos(Model m, glm::vec3 newPos);
    void setObjVel(Model m, glm::vec3 newVel);
    void setObjAcc(Model m, glm::vec3 newAcc);

    //nudge values
    void nudge(Model m, glm::vec3 nudge);

private:
    float deltaTime;
    float lastFrame;
    float charFriction = 0.5f;
    float charGravity = 9.8f;
    std::vector<glm::vec3> charhitbox = {
        glm::vec3(0.5f, 0.0f, 0.5f),
        glm::vec3(0.5f, 0.0f, -0.5f),
        glm::vec3(-0.5f, 0.0f, -0.5f),
        glm::vec3(-0.5f, 0.0f, 0.5f),
        glm::vec3(0.5f, 1.0f, 0.5f),
        glm::vec3(0.5f, 1.0f, -0.5f),
        glm::vec3(-0.5f, 1.0f, -0.5f),
        glm::vec3(-0.5f, 1.0f, 0.5f)
    };
    float charSpeed;
    bool charJump;
    bool charCrouch;
    bool charSprint;
    bool charForward;
    bool charBackward;
    bool charLeft;
    bool charRight;
    bool charGrounded;
    float rigidFriction = 0.5f;
    float rigidGravity = -9.8f;
};

#endif /* physics_hpp */