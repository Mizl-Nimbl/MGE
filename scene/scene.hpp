#ifndef scene_hpp
#define scene_hpp

#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyxml2.h>

#include "../model/model.hpp"
#include "../declarations.hpp"

class Scene
{
public:
    Scene();
    Scene(glm::vec3 loc, glm::vec3 rot, glm::vec3 sca);
    Scene(std::string path);

    std::vector<Model> getModels();
    Model getModel(int index);
    glm::vec3 getGlobalLocation();
    glm::vec3 getGlobalRotation();
    glm::vec3 getGlobalScale();
    glm::vec3 getModelLocation(int index);
    glm::vec3 getModelRotation(int index);
    glm::vec3 getModelScale(int index);

    void setModel(std::string const &modelpath);
    void setGlobalLocation(glm::vec3 newLocation);
    void setGlobalRotation(glm::vec3 newRotation);
    void setGlobalScale(glm::vec3 newScale);
    void setModelLocation(int index, glm::vec3 newLocation);
    void setModelRotation(int index, glm::vec3 newRotation);
    void setModelScale(int index, glm::vec3 newScale);
private:
    std::vector<Model> models;
    glm::vec3 location;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif /* scene_hpp */

