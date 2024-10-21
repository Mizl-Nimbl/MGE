#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 Pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    Pos = lightSpaceMatrix * model * vec4(aPos, 1.0);
}