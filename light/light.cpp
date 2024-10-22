#include "light.hpp"

Light::Light(){}

Light::Light(int re, int gr, int bl, int al, glm::vec3 adirection)
{
    type = 0;
    bright = (al / 100.0f);
    bright += 1.0f;
    bright *= 2.0f;
    red    = (re / 255.0f) * bright;
    green  = (gr / 255.0f) * bright;
    blue   = (bl / 255.0f) * bright;
}

Light::Light(int re, int gr, int bl, int al, glm::vec3 aposition, float attentuation)
{
    type = 1;
    bright = (al / 100.0f);
    bright += 1.0f;
    bright *= 2.0f;
    red    = (re / 255.0f) * bright;
    green  = (gr / 255.0f) * bright;
    blue   = (bl / 255.0f) * bright;
    position = aposition;
    constant = 1.0f * (attentuation * bright);
    linear = 0.09f * (attentuation * bright);
    quadratic = 0.032f * (attentuation * bright);
}

Light::Light(int re, int gr, int bl, int al, glm::vec3 aposition, glm::vec3 adirection, float attenuation)
{
    type = 2;
    bright = (al / 100.0f);
    bright += 1.0f;
    bright *= 2.0f;
    red    = (re / 255.0f) * bright;
    green  = (gr / 255.0f) * bright;
    blue   = (bl / 255.0f) * bright;
    position = aposition;
    direction = adirection;
    constant = 1.0f * (attenuation * bright);
    linear = 0.09f * (attenuation * bright);
    quadratic = 0.032f * (attenuation * bright);
    cutOff = glm::cos(glm::radians(12.5f));
    outerCutOff = glm::cos(glm::radians(17.5f));
}

void Light::setDirection(glm::vec3 adirection)
{
    direction = adirection;
}

void Light::setPosition(glm::vec3 aposition)
{
    position = aposition;
}

void Light::setConstant(float aconstant)
{
    constant = 1.0f * aconstant;
}

void Light::setLinear(float alinear)
{
    linear = 0.09f * alinear;
}

void Light::setQuadratic(float aquadratic)
{
    quadratic = 0.032f * aquadratic;
}

void Light::setCutOff(float acutOff)
{
    cutOff = glm::cos(glm::radians(acutOff));
}

void Light::setOuterCutOff(float aouterCutOff)
{
    outerCutOff = glm::cos(glm::radians(aouterCutOff));
}

void Light::setAlpha(int al)
{
    bright = (al / 255.0f);
}

void Light::setRed(int re)
{
    red = (re / 255.0f) * bright;
}

void Light::setGreen(int gr)
{
    green = (gr / 255.0f) * bright;
}

void Light::setBlue(int bl)
{
    blue = (bl / 255.0f) * bright;
}

glm::vec3 Light::getDirection()
{
    return direction;
}

glm::vec3 Light::getPosition()
{
    return position;
}

float Light::getConstant()
{
    return constant;
}

float Light::getLinear()
{
    return linear;
}

float Light::getQuadratic()
{
    return quadratic;
}

float Light::getCutOff()
{
    return cutOff;
}

float Light::getOuterCutOff()
{
    return outerCutOff;
}

float Light::getAlpha()
{
    return bright;
}

float Light::getRed()
{
    return red;
}

float Light::getGreen()
{
    return green;
}

float Light::getBlue()
{
    return blue;
}

int Light::getType()
{
    return type;
}