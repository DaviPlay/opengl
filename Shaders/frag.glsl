#version 410 core

#define NR_POINT_LIGHTS 1

out vec4 FragColor;

in vec2 textureCoords;
in vec3 fragPos;
in vec3 fragNormal;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir);

void main()
{
    vec3 norm    = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);

    result += CalcSpotLight(spotLight, norm, fragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir   = normalize(-light.direction);
    float diff      = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient    = light.ambient  * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 diffuse    = light.diffuse  * diff * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 specular   = light.specular * spec * vec3(texture(material.texture_specular, textureCoords));
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 pos, vec3 viewDir)
{
    vec3 lightDir   = normalize(light.position - pos);
    float diff      = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance    = length(light.position - pos);
    float denom       = max(light.constant + light.linear * distance + light.quadratic * distance * distance, 0.001);
    float attenuation = 1.0 / denom;
    vec3 ambient    = light.ambient  * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 diffuse    = light.diffuse  * diff * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 specular   = light.specular * spec * vec3(texture(material.texture_specular, textureCoords));
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 pos, vec3 viewDir)
{
    vec3 lightDir   = normalize(light.position - pos);
    float diff      = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance    = length(light.position - pos);
    float denom       = max(light.constant + light.linear * distance + light.quadratic * distance * distance, 0.001);
    float attenuation = 1.0 / denom;
    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = epsilon != 0.0 ? clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0) : 0.0;
    vec3 ambient    = light.ambient  * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 diffuse    = light.diffuse  * diff * vec3(texture(material.texture_diffuse,  textureCoords));
    vec3 specular   = light.specular * spec * vec3(texture(material.texture_specular, textureCoords));
    return (ambient + diffuse + specular) * attenuation * intensity;
}