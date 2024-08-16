#ifndef textures_hpp
#define textures_hpp

//include gl
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"
#include "../declarations.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Textures
{
public:
    unsigned int initTexture(const char* texturepath);
    unsigned int initCubemap(std::vector<std::string> faces);
private:
    std::unordered_map<std::string, unsigned int> textureCache;
};

#endif /* textures_hpp */