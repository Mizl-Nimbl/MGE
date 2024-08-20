#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D ssao;

const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1  
    );

    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    col *= 0.2;

    // Mix col with white color to reduce its influence
    const float gamma = 1.3;
    vec3 hdrColour = texture(screenTexture, TexCoords).rgb * texture(ssao, TexCoords).r;
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));

    // Blend edge detection result with hdrColour
    float edgeOpacity = 0.001; // Adjust this value to control the opacity of the edge detection
    vec3 finalColor = mix(mapped, col, edgeOpacity);

    FragColor = vec4(finalColor, 1.0);
}