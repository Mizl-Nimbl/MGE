#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // attenuation values
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    // 0 = directional, 1 = point, 2 = spot
    int type;
};

in vec3 FragPos;
in vec2 Texture;
in mat3 TBN;
in vec4 FragPosLightSpace;

#define lightCount 16
#define rayCount 1
#define maxReflections 8
#define maxObjects 16

uniform vec3 viewPos;
uniform Material material;
uniform Light light[lightCount];
uniform sampler2D shadowMap;
uniform sampler2D ssaoMap;

vec3 RayCast(vec3 origin, vec3 direction)
{
    vec3 hitPoint = vec3(0.0);
    for (int i = 0; i < maxObjects; i++)
    {
        //replace with ray tracing code
        hitPoint = vec3(0.0);
        
    }
    return hitPoint;
}

vec3 TraceRayReflections(vec3 startingOrigin, vec3 startingDirection)
{
    vec3 result = vec3(0.0);
    for (int j = 0; j < maxReflections; j++)
    {
        //replace with ray tracing code
        vec3 hitPoint = RayCast(startingOrigin, startingDirection);
        vec3 normal = normalize(hitPoint - texture(material.normal, Texture).rgb); // Assume a normal for the hit point

        for (int i = 0; i < lightCount; i++)
        {
            vec3 lightDir = normalize(light[i].position - hitPoint);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 viewDir = normalize(viewPos - hitPoint);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 ambient = light[i].ambient * texture(material.diffuse, Texture).rgb;
            vec3 diffuse = light[i].diffuse * diff * texture(material.diffuse, Texture).rgb;
            vec3 specular = light[i].specular * spec * texture(material.specular, Texture).rgb;
            result += (ambient + diffuse + specular);
        }

        vec3 reflectDir = reflect(-direction, normal);
        vec3 origin = hitPoint;
    }
    return result;
}

void main()
{
    vec4 result = vec4(0.0);

    result.rgb += TraceRayReflections();

    result.a = texture(material.diffuse, Texture).a;

    FragColor = vec4(result);
}
