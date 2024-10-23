#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
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
uniform vec3 viewPos;
uniform Material material;
uniform Light light[lightCount];
uniform sampler2D shadowMap;

void main()
{
    //vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    //projCoords = projCoords * 0.5 + 0.5;
    //float closestDepth = texture(shadowMap, projCoords.xy).w * 100.0;
    //float currentDepth = length(FragPos.xyz - FragPosLightSpace.xyz);
    //float shadow = currentDepth + 0.001 > closestDepth ? 1.0 : 0.0;
    vec4 result = vec4(0.0);
    for(int i = 0; i < lightCount; i++)
    {
        float theta;
        float epsilon;
        float intensity;
        float distance;
        float attenuation = 1.0;
        vec3 lightDir;
        vec3 halfwayDir;
        vec3 viewDir;
        vec3 norm;
        float diff;
        float spec;

        viewDir = normalize((TBN * viewPos) - FragPos);
        norm = texture(material.texture_normal, Texture).rgb;
        
        norm = normalize(TBN * norm); // Transform to world space
        norm = normalize(norm * 2.0 - 1.0); // Transform from [0,1] to [-1,1]

        // diffuse
        if (light[i].type == 0)
        {
            attenuation = 1.0;
            lightDir = normalize(TBN * -light[i].direction);
        }
        else
        {
            lightDir = normalize((TBN * light[i].position) - FragPos);
            distance = length(TBN * light[i].position - FragPos);
            attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
        }

        // ambient
        vec4 ambient = vec4(0.0);
        vec4 diffuse = vec4(0.0);
        vec4 specular = vec4(0.0);

        vec4 diffColor = texture(material.texture_diffuse, Texture).rgba;
        ambient += vec4(light[i].ambient, 1.0) * diffColor;
        halfwayDir = normalize(lightDir + viewDir);
        diff = max(dot(halfwayDir, norm), 0.0);
        vec4 specColor = texture(material.texture_specular, Texture).rgba;
        float shininess = ((specColor.r + specColor.g + specColor.b) / 3.0);
        spec = pow(max(dot(halfwayDir, norm), 0.0), shininess);
        diffuse += vec4(light[i].diffuse, 1.0) * diff * diffColor;
        specular += vec4(light[i].specular, 1.0) * spec * specColor;

        if(light[i].type == 2)
        {
            theta = dot(lightDir, normalize(-light[i].direction));
            epsilon = light[i].cutOff;
            intensity = clamp((theta - epsilon) / (1 - epsilon), 0, 1);
            diffuse *= intensity;
            specular *= intensity;
        }

        // attenuation apply
        if (light[i].type == 1 || light[i].type == 2)
        {
            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;
        }

        ambient *= 0.1;
        
        result += (ambient + diffuse + specular);
        //result += (ambient + (1.0 - shadow) * (diffuse + specular));
        //result += specular;
    }
    result.a = texture(material.texture_diffuse, Texture).a;
    FragColor = vec4(result);
}