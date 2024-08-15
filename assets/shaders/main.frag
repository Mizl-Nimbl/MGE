#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse[16];
    sampler2D specular[16];
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
in vec3 Normal;
in vec2 Texture;

#define lightCount 16
uniform vec3 viewPos;
uniform Material material;
uniform Light light[lightCount];

void main()
{
    vec4 result = vec4(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    for(int i = 0; i < lightCount; i++)
    {
        float theta;
        float epsilon;
        float intensity;
        float distance;
        float attenuation = 1.0;
        vec3 lightDir;
        vec3 halfwayDir;

        // attenuation calculations
        if (light[i].type == 1 || light[i].type == 2)
        {
            distance = length(light[i].position - FragPos);
            attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
        }

        // ambient
        vec4 ambient = vec4(0.0);
        for (int j = 0; j < 16; j++) {
            vec4 texColor = texture(material.diffuse[j], Texture).rgba;
            ambient += vec4(light[i].ambient, 1.0) * texColor;
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
        }
        halfwayDir = normalize(lightDir + viewDir);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, halfwayDir), 0.0), (material.shininess) * 3.0);

        vec4 diffuse = vec4(0.0);
        vec4 specular = vec4(0.0);
        for (int j = 0; j < 16; j++) {
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
        
        result += (ambient + diffuse + specular);
    }
    for (int j = 0; j < 16; j++) {
        result.a = texture(material.diffuse[j], Texture).a;
    }
    FragColor = vec4(result);
}
