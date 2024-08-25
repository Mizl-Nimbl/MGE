#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 resolution;

void main()
{
    const float gamma = 0.6;
    vec3 hdrColour = texture(screenTexture, TexCoords).rgb;
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));
    mapped = pow(mapped, vec3(1.0 / gamma));
    float gridsize = 6.0;
    vec2 uv = TexCoords * resolution;
    vec2 grid = vec2(gridsize); // Adjust the grid size as needed
    vec2 pos = mod(uv, grid) - grid * 0.5;
    vec4 pixel = vec4(mapped, 1.0);
    const vec3 GREY_WEIGHTS = vec3(0.299, 0.587, 0.114);
    float greyResult = dot(pixel.rgb, GREY_WEIGHTS);
    float radius = (1.0 - greyResult) * 0.5 * length(grid);
    float dist = length(pos);
    float c = smoothstep(radius, radius + 1.0, dist);
    vec3 color = mix(vec3(0.0), pixel.rgb, c);
    FragColor = vec4(color, pixel.a);
}