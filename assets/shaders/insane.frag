#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse[16];
    sampler2D specular[16];
    sampler2D normal[16];
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
        float shadow;

        for (int j = 0; j < 16; j++) 
        {
            viewDir = normalize(viewPos - FragPos);
            norm = texture(material.normal[j], Texture).rgb;
            //norm = normalize(norm * 2.0 - 1.0); // Transform from [0,1] to [-1,1]
            norm = normalize(TBN * norm); // Transform to world space
        }

        // diffuse
        if (light[i].type == 0)
        {
            attenuation = 1.0;
            lightDir = normalize(-light[i].direction);
        }
        else
        {
            lightDir = normalize(light[i].position - FragPos);
            distance = length(light[i].position - FragPos);
            attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
        }

        // ambient
        vec4 ambient = vec4(0.0);
        for (int j = 0; j < 16; j++) {
            vec4 texColor = texture(material.diffuse[j], Texture).rgba;
            ambient += vec4(light[i].ambient, 1.0) * texColor;
        }

        
        halfwayDir = normalize(lightDir + viewDir);
        vec3 reflectDir = reflect(-lightDir, norm);
        diff = max(dot(norm, lightDir), 0.0);
        vec3 blendedDir = normalize(mix(reflectDir, halfwayDir, 0.5));
        

        vec4 diffuse = vec4(0.0);
        vec4 specular = vec4(0.0);
        for (int j = 0; j < 16; j++) {
            float shininess = texture(material.specular[j], Texture).r;
            spec = pow(max(dot(viewDir, blendedDir), 0.0), (shininess) * 7.0);
            vec4 texColor = texture(material.diffuse[j], Texture).rgba;
            vec4 texSpec = texture(material.specular[j], Texture).rgba;
            diffuse += vec4(light[i].diffuse, 1) * diff * texColor;
            specular += vec4(light[i].specular, 1) * spec * texSpec;
        }

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
        
        result += (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    for (int j = 0; j < 16; j++) {
        result.a = texture(material.diffuse[j], Texture).a;
    }

    vec3 center = vec3(0.25);
    float radius = 0.5;
    float distance = length(FragPos - center);
    if (distance > radius)
    {
        discard;
    }

    FragColor = vec4(result);
}