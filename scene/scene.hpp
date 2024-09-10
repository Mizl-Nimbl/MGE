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
#include "../audio/audio.hpp"
#include "../font/font.hpp"
#include "../audiobank/audiobank.hpp"
#include "../signalemitter/signalemitter.hpp"
#include "../declarations.hpp"

class Scene
{
public:
    Scene();
    Scene(glm::vec3 loc, glm::vec3 rot, glm::vec3 sca);
    Scene(std::string path);

    std::vector<Model> getModels();
    std::vector<Font> getTexts();
    int getPhysicsType();
    Model getModel(int index);
    glm::vec3 getGlobalLocation();
    glm::vec3 getGlobalRotation();
    glm::vec3 getGlobalScale();
    glm::vec3 getModelLocation(int index);
    glm::vec3 getModelRotation(int index);
    glm::vec3 getModelScale(int index);
    glm::vec2 getTextLocation(int index);
    float getTextRotation(int index);
    float getTextScale(int index);
    glm::vec3 getTextColor(int index);

    void setModel(std::string const &modelpath);
    void setPhysicsType(int newType);
    void setText(Font& f, std::string text);
    void setGlobalLocation(glm::vec3 newLocation);
    void setGlobalRotation(glm::vec3 newRotation);
    void setGlobalScale(glm::vec3 newScale);
    void setModelLocation(int index, glm::vec3 newLocation);
    void setModelRotation(int index, glm::vec3 newRotation);
    void setModelScale(int index, glm::vec3 newScale);
    void setTextLocation(Font& f, glm::vec2 newLocation);
    void setTextRotation(Font& f, float newRotation);
    void setTextScale(Font& f, float newScale);
    void setTextColor(Font& f, glm::vec3 newColor);
private:
    std::vector<Model> models;
    std::vector<Audio*> audios;
    std::vector<Font> texts;
    int physicsType;
    glm::vec3 location;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif /* scene_hpp */

