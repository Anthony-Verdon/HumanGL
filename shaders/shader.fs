#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_MATERIALS 100
uniform Material materials[MAX_MATERIALS];
uniform Light light;
uniform vec3 viewPos; 

in vec2 TexCoord;
in vec3 color;
in vec3 Normal; 
in vec3 FragPos; 
in float MaterialIndex;
out vec4 FragColor;

void main()
{
    vec3 ambient = light.ambient * materials[int(MaterialIndex)].ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * materials[int(MaterialIndex)].diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[int(MaterialIndex)].shininess);
    vec3 specular = light.specular * spec * materials[int(MaterialIndex)].specular;   

    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);
}