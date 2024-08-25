#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 Texture;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    Texture = aTexture;
    FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 N = normalize(mat3(model) * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(mat3(model) * aBitangent);
    TBN = mat3(T, B, N);
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}