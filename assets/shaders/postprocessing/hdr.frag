#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screen;
uniform sampler2D ssao;

void main()
{
    vec3 color = texture(screen, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
}