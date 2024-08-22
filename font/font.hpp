#ifndef font_hpp
#define font_hpp

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../systems/systems.hpp"
#include "../declarations.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    unsigned int textureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class Font
{
public:
    Font() = default;
    Font(std::string path);
    std::string getText();
    glm::vec2 getPosition();
    float getRotation();
    float getScale();
    std::map<char, Character> getCharacters();
    glm::vec3 getColor();
    void setText(std::string ntext);
    void setPosition(glm::vec2 pos);
    void setRotation(float rot);
    void setScale(float scale);
    void setColor(glm::vec3 col);
private:
    std::map<char, Character> characters;
    glm::vec2 textPos;
    std::string text;
    float textRot;
    float textScale;
    glm::vec3 textColor;
};

#endif /* font_hpp */