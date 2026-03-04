#version 410 core

out vec4 FragColor;

in vec2 textureCoords;
in vec3 fragPos;
in vec3 normal;

struct Material {
   sampler2D texture_diffuse;
   sampler2D texture_specular;
   sampler2D texture_normal;
   sampler2D texture_height;
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

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float linearizeDepth(float depth);

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform Material material;
uniform float mixAmount;
uniform vec3 viewPos;

float near = 0.1;
float far = 100.0;

void main()
{
   vec3 norm = normalize(normal);
   vec3 viewDir = normalize(viewPos - fragPos);

   vec3 result = calcDirLight(dirLight, norm, viewDir);

   //for (int i = 0; i < NR_POINT_LIGHTS; i++)
   //   result += calcPointLight(pointLights[i], norm, fragPos, viewDir);

   // depth testing
   //FragColor = texture(material.texture_diffuse, textureCoords);
   //float depth = linearizeDepth(gl_FragCoord.z) / far;
   //FragColor = vec4(vec3(depth), 1.0);

   FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
   vec3 lightDir = normalize(-light.direction);
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, textureCoords));
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, textureCoords));
   vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, textureCoords));

   return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
   vec3 lightDir = normalize(light.position - fragPos);
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // attenuation
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + (light.quadratic * pow(distance, 2)));

   vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, textureCoords));
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, textureCoords));
   vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, textureCoords));
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;

   return (ambient + diffuse + specular);
}

float linearizeDepth(float depth)
{
   float z = depth * 2.0 - 1.0; // back to NDC
   return (2.0 * near * far) / (far + near - z * (far - near));
}