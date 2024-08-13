#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

out vec2 Texture;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    Texture = aTexture;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}