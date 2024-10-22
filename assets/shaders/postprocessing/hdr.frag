#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screen;
uniform sampler2D ssao;

void main()
{
    const float gamma = 1.5;
    vec3 hdrColor = texture(screen, TexCoords).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}