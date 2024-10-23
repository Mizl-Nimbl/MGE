#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 Texture;
out vec3 FragPos;
out vec3 TangentFragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    Texture = aTexture;
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TangentFragPos = FragPos;
}