#include "physics.hpp"

Physics::Physics()
{
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
    for(int i = 0; i < scenes.size(); i++)
    {
        if(scenes[i].getPhysicsType() == 1)
        {
            Physicsmaths(scenes[i]);
        }
        Collisions(scenes[i]);
    }
    Movement();
}

void Physics::Collisions(Scene scene)
{
    //collison, friction, rotation, etc
    //for each model:
    for (int i = 0; i < scene.getModels().size(); i++)
    {
        std::vector<glm::vec3> modelBox = scene.getModels()[i].getBox();
        //check for box's collisions with charhibox
        for (int j = 0; j < modelBox.size(); j++)
        {
            for (int k = 0; k < charhitbox.size(); k++)
            {
                if (modelBox[j].x == charhitbox[k].x && modelBox[j].z == charhitbox[k].z)
                {
                    if (modelBox[j].y == charhitbox[k].y)
                    {
                        charGrounded = true;
                    }
                    else
                    {
                        charGrounded = false;
                    }
                }
            }
        }
        //check for box's collisions with rigidbodies
        //check for box's collisions with staticbodies
    }
}

void Physics::Physicsmaths(Scene scene)
{
    //gravety on rigidbodies
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
    //gravety on characters if not colliding with static and rigid bodies
    /*
    if (!charGrounded)
    {
        r.cameraPos.y -= charGravity * deltaTime;
    }
    else if (charGrounded)
    {
        r.cameraPos.x *= charFriction * deltaTime;
        r.cameraPos.z *= charFriction * deltaTime;
    }
    */
}
