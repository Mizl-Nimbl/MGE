#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screen;
uniform sampler2D ssao;

void main()
{
    const float gamma = 1.9;
    vec3 hdrColour = texture(screen, TexCoords).rgb * (texture(ssao, TexCoords).r);
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));
    const float saturation = 2.0;
    mapped = mix(vec3(dot(mapped, vec3(0.2126, 0.7152, 0.0722))), mapped, saturation);

    FragColor = vec4(mapped, 1.0);
}