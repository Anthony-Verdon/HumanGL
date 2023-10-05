#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aTexCoord;

uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoord;

void main()
{
    gl_Position = projection * view * rotation * aPos;
    TexCoord = aTexCoord;
}