#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //attenuation values
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    //0 = directional, 1 = point, 2 = spot
    int type;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;

#define lightCount 100
uniform vec3 viewPos;
uniform Material material;
uniform Light light[lightCount];

void main()
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    for(int i = 0; i < lightCount; i++)
    {
        float theta;
        float epsilon;
        float intensity;
        float distance;
        float attenuation;
        vec3 lightDir;
        //attenuation calculations
        if (light[i].type == 1 || light[i].type == 2)
        {
            distance = length(light[i].position - FragPos);
            attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
        }
        // ambient
        vec3 ambient = light[i].ambient * texture(material.diffuse, Texture).rgb;
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
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
        vec3 diffuse = light[i].diffuse * diff * texture(material.diffuse, Texture).rgb;
        // specular
        vec3 specular = light[i].specular * spec * texture(material.specular, Texture).rgb;
        if(light[i].type == 2)
        {
            theta = dot(lightDir, normalize(-light[i].direction));
            epsilon = light[i].cutOff;
            intensity = clamp((theta - epsilon) / (1 - epsilon), 0, 1);
            diffuse *= intensity;
            specular *= intensity;
        }
        //attenuation apply
        if (light[i].type == 1 || light[i].type == 2)
        {
            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;
        }
        result += (ambient + diffuse + specular);
    }
    FragColor = vec4(result, 1.0);
}
