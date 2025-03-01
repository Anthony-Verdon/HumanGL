#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoord;
out vec3 color;
out vec3 Normal;
out vec3 FragPos; 

void main()
{
    gl_Position = projection * view * rotation * model * aPos;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    color = aColor;
    Normal = mat3(transpose(inverse(rotation *model))) * aNormal; 
    FragPos = vec3(model * aPos);
}