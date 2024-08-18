#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screen;
uniform sampler2D ssao;

void main()
{   
    const float gamma = 1.3;
    vec3 hdrColour = texture(screen, TexCoords).rgb * (texture(ssao, TexCoords).r);
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));
    FragColor = vec4(mapped, 1.0);
}