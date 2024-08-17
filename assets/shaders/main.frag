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

            //vec3 templightDir = normalize(-light[i].direction);
            //float bias = max(0.05 * (1.0 - dot(norm, templightDir)), 0.005);  
            //vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
            //projCoords = projCoords * 0.5 + 0.5; // Transform to [0,1] range
            //float closestDepth = texture(shadowMap, projCoords.xy).r;
            //float currentDepth = projCoords.z;
            //shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
            //if (projCoords.x > 1.0 || projCoords.y > 1.0 || projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.y < 0.0 || projCoords.z < 0.0)
            //{
            //    shadow = 0.0;
            //}
        }

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
        for (int j = 0; j < 16; j++) {
            halfwayDir = normalize(lightDir + viewDir);
            diff = max(dot(norm, lightDir), 0.0);
            //vec3 reflectDir = reflect(-lightDir, norm);
            spec = pow(max(dot(viewDir, halfwayDir), 0.0), (material.shininess) * 3.0);
        }

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
