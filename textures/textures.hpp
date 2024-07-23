#ifndef textures_hpp
#define textures_hpp

//include gl
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"
#include "../declarations.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Textures
{
public:
    unsigned int initTexture(const char* texturepath);
private:
};

#endif /* textures_hpp */