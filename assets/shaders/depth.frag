#version 330 core

layout(location = 0) out float fragmentdepth;

in vec4 Pos;

void main()
{             
    fragmentdepth = Pos.z;
}