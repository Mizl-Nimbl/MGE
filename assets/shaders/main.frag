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
#define rayCount 2
#define maxReflections 6
#define maxObjects 16

uniform vec3 viewPos;
uniform Material material;
uniform Light light[lightCount];
uniform sampler2D shadowMap;

float hash(float n) {
    return fract(sin(n) * 43758.5453123);
}

vec3 randomVec3(float seed) {
    return vec3(hash(seed), hash(seed + 1.0), hash(seed + 2.0));
}

vec3 hit(vec3 ray, vec3 direction) 
{
    vec3 oc = ray - FragPos;
    float radius = length(FragPos - ray) * 0.3;
    float a = dot(direction, direction);
    float b = 2.0 * dot(oc, direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant > 0.0) {
        float sqrtDiscriminant = sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0 * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0 * a);

        if (t1 > 0.0) {
            return ray + t1 * direction;
        } else if (t2 > 0.0) {
            return ray + t2 * direction;
        }
    }

    return vec3(0.0);
}

vec3 calculateLight(vec3 pos, vec3 dir, vec3 amb, vec3 dif, vec3 spe, float con, float lin, float qua, float cut, int typ, vec3 normal, vec3 lightDir, vec3 hit)
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 diffuseColor = texture(material.diffuse[0], Texture).rgb;
    vec3 specularColor = vec3(0.5);
    vec3 norm = normal;

    float intensityFactor = 1.2;

    //ambient = diffuseColor * 0.1;

    if (typ == 0) { // Directional light
        lightDir = normalize(-dir - hit);
    } else {
        lightDir = normalize(pos - hit);
    }

    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    diffuse = dif * diff * diffuseColor * intensityFactor;

    vec3 viewDir = normalize(hit - viewPos);
    if (dot(norm, viewDir) < 0.0) {
        norm = -norm; // Flip the normal if it's facing away
    }
    vec3 reflectDir = reflect(-lightDir, norm);
    float shininess = texture(material.specular[0], Texture).r + 1.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), (shininess) * 256.0);

    // Specular shading
    specular = spe * spec * specularColor * intensityFactor;

    // Attenuation
    float dist = length(pos - FragPos);
    float attenuation = 1.0;
    if (typ != 0) { // Point or spot light
        attenuation = 1.0 / max(con + lin * dist + qua * (dist * dist), 0.001);
    }

    // Spotlight (soft edges)
    if (typ == 2) {
        float theta = dot(lightDir, normalize(-dir));
        float epsilon = cut - 0.01;
        float intensity = clamp((theta - epsilon) / (cut - epsilon), 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
    }

    //highp vec3 vibrantAmbient = makeLightVibrant(ambient, 1.9, 1.2, 2.0);
    //highp vec3 vibrantDiffuse = makeLightVibrant(diffuse, 1.9, 1.2, 2.0);
    //highp vec3 vibrantSpecular = makeLightVibrant(specular, 1.9, 1.2, 2.0);

    vec3 result = (ambient + diffuse + specular) * attenuation;
    result = max(result, vec3(0.0));
    result *= vec3(0.3);

    return result;
}

vec3 rayCast(vec3 origin, vec3 direction, out vec3 hitPoint, out vec3 normal, out vec3 hitColor)
{
    vec3 ray = origin;
    vec3 closestColor = vec3(0.001);
    vec3 planeNormal = vec3(0.0);
    vec3 closestHitPoint = vec3(0.0);
    float closestDistance = 1e30;
    bool hitDetected = false;

    for(int i = 0; i < rayCount; i++)
    {
        vec3 tangentNormal = texture(material.normal[0], Texture).rgb * 2.0 - 1.0;
        vec3 planeNormal = normalize(TBN * tangentNormal);
        vec3 lighthit = hit(ray, direction);
        if(lighthit != vec3(0.0))
        {
            float dist = length(lighthit - ray);
            if(dist < closestDistance)
            {
                closestDistance = dist;
                closestHitPoint = lighthit;
                //closestColor += texture(material.diffuse[0], Texture).rgb * 0.1;
                for (int k = 0; k < lightCount; k++)
                {
                    vec3 lightDir = normalize(light[k].position - closestHitPoint);
                    closestColor += calculateLight(light[k].position, light[k].direction, light[k].ambient, light[k].diffuse, light[k].specular, light[k].constant, light[k].linear, light[k].quadratic, light[k].cutOff, light[k].type, planeNormal, lightDir, closestHitPoint);
                    hitDetected = true;
                }
            }
        }
        if (hitDetected)
        {
            break;
        }
    }

    hitPoint = closestHitPoint;
    normal = planeNormal;
    hitColor = closestColor;

    float roughness = texture(material.specular[0], Texture).r;

    vec3 noise = randomVec3(float(gl_FragCoord.x * gl_FragCoord.y));
    vec3 roughDirection = normalize(direction + (noise - vec3(0.5)) * roughness);

    vec3 reflectionColor = vec3(0.0);
    for(int i = 0; i < rayCount; i++)
    {
        vec3 lighthit = hit(ray, roughDirection);
        if(lighthit != vec3(0.0))
        {
            float dist = length(lighthit - ray);
            if(dist < closestDistance)
            {
                closestDistance = dist;
                closestHitPoint = lighthit;
                for (int k = 0; k < lightCount; k++)
                {
                    vec3 lightDir = normalize(light[k].position - closestHitPoint);
                    reflectionColor += calculateLight(light[k].position, light[k].direction, light[k].ambient, light[k].diffuse, light[k].specular, light[k].constant, light[k].linear, light[k].quadratic, light[k].cutOff, light[k].type, planeNormal, lightDir, closestHitPoint);
                }
            }
        }
    }

    hitColor = mix(hitColor, reflectionColor, roughness * 2.0);

    return closestColor;
}

vec3 TraceRayReflections(vec3 origin, vec3 direction)
{
    vec3 color = vec3(0.0);
    vec3 currentOrigin = origin;
    vec3 currentDirection = direction;
    float attenuation = 1.0;

    for (int reflections = 0; reflections < maxReflections; reflections++)
    {
        vec3 hitPoint;
        vec3 normal;
        vec3 hitColor;
        vec3 ray = rayCast(currentOrigin, currentDirection, hitPoint, normal, hitColor);
        if (length(hitPoint) == 0.0)
        {
            break;
        }
        color += hitColor * attenuation;
        currentOrigin = hitPoint;
        currentDirection = reflect(currentDirection, normal);
        attenuation *= 0.8;
    }
    return color;
}


void main()
{
    vec4 result = vec4(0.0);
    result.rgb = TraceRayReflections(viewPos, normalize(FragPos - viewPos));
    result.a = texture(material.diffuse[0], Texture).a;
    FragColor = vec4(result);
}
