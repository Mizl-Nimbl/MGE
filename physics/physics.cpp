#include "physics.hpp"

Physics::Physics()
{
    //signal connections
    e.connect(10, [this]() { charForward = true; });
    e.connect(20, [this]() { charBackward = true; });
    e.connect(30, [this]() { charLeft = true; });
    e.connect(40, [this]() { charRight = true; });
    e.connect(50, [this]() { charJump = true; });
    e.connect(60, [this]() { charCrouch = true; });
    e.connect(70, [this]() { charSprint = true; });
}

void Physics::ProcessPhysics(std::vector<Scene> scenes)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    Movement();
}

void Physics::Movement()
{
    //handle movement
    if (charSprint)
    {
        charSpeed = 12.5f * deltaTime;
    }
    else
    {
        charSpeed = 6.5f * deltaTime;
    }
    if (charForward)
    {
        r.cameraPos += charSpeed * r.cameraFront;
    }
    if (charBackward)
    {
        r.cameraPos -= charSpeed * r.cameraFront;
    }
    if (charLeft)
    {
        r.cameraPos -= glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * charSpeed;
    }
    if (charRight)
    {
        r.cameraPos += glm::normalize(glm::cross(r.cameraFront, r.cameraUp)) * charSpeed;
    }
    if (charJump)
    {
        //jump physics
        //r.cameraPos += charSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        if (charGrounded)
        {
            r.cameraPos.y += charSpeed * deltaTime;
        }
    }
    if (charCrouch)
    {
        r.cameraPos.y -= 0.5f;
    }
    charForward = false;
    charBackward = false;
    charLeft = false;
    charRight = false;
    charJump = false;
    charCrouch = false;
    charSprint = false;
}

//getters

glm::vec3 Physics::getObjPos(Model m)
{
    return m.getLocation();
}

glm::vec3 Physics::getObjVel(Model m)
{
    if (m.getLastLocation() != glm::vec3(0.0f))
    {
        m.setLastLocation(getObjPos(m));
    }
    glm::vec3 velocity = (getObjPos(m) - m.getLastLocation()) / deltaTime;
    m.setLastLocation(getObjPos(m));
    return velocity;
}

glm::vec3 Physics::getObjAcc(Model m)
{
    if (m.getLastVelocity() == glm::vec3(0.0f))
    {
        m.setLastVelocity(getObjVel(m));
    }
    glm::vec3 acceleration = (getObjVel(m) - m.getLastVelocity()) / deltaTime;
    m.setLastVelocity(getObjVel(m));
    return acceleration;
}

//setters

void Physics::setObjPos(Model m, glm::vec3 newPos)
{
    m.setLocation(newPos);
}

void Physics::setObjVel(Model m, glm::vec3 newVel)
{
    m.setLastVelocity(newVel);
}

void Physics::setObjAcc(Model m, glm::vec3 newAcc)
{
    m.setLastVelocity(newAcc);
}

//nudge values

void Physics::nudge(Model m, glm::vec3 nudge)
{
    m.setLastVelocity(nudge + getObjVel(m));
}