#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screen;
uniform sampler2D ssao;

void main()
{
    const float gamma = 1.4;
    const float saturation = 1.4;

    vec3 hdrColor = texture(screen, TexCoords).rgb;

    hdrColor.b *= 1.1;

    // Reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    
    // Convert to grayscale
    float luminance = dot(mapped, vec3(0.2126, 0.7152, 0.0722));
    vec3 gray = vec3(luminance);
    
    // Adjust saturation
    mapped = mix(gray, mapped, saturation);
    
    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}