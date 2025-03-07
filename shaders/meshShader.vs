#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aJoint;
layout (location = 4) in vec4 aWeight;

uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoord;
out vec3 Normal;

void main()
{
    gl_Position = projection * view * rotation * model * vec4(aPos, 1);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    Normal = mat3(transpose(inverse(rotation * model))) * aNormal; 
}