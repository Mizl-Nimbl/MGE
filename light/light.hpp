#ifndef light_hpp
#define light_hpp

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "../shaders/shader.hpp"
#include "../declarations.hpp"

class Light
{
public:
    Light();
    Light(int re, int gr, int bl, int al, glm::vec3 adirection);
    Light(int re, int gr, int bl, int al, glm::vec3 aposition, float attenuation);
    Light(int re, int gr, int bl, int al, glm::vec3 aposition, glm::vec3 adirection, float attenuation);

    void setDirection(glm::vec3 adirection);
    void setPosition(glm::vec3 aposition);
    void setConstant(float aconstant);
    void setLinear(float alinear);
    void setQuadratic(float aquadratic);
    void setCutOff(float acutOff);
    void setOuterCutOff(float aouterCutOff);
    void setAlpha(int al);
    void setRed(int re);
    void setGreen(int gr);
    void setBlue(int bl);

    glm::vec3 getDirection();
    glm::vec3 getPosition();
    float getConstant();
    float getLinear();
    float getQuadratic();
    float getCutOff();
    float getOuterCutOff();
    float getAlpha();
    float getRed();
    float getGreen();
    float getBlue();
    int getType();
private:
    glm::vec3 position;
    glm::vec3 direction;
    float bright;
    int type;
    float red;
    float green;
    float blue;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#endif /* light_hpp */