#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aColor;

uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 color;

void main()
{
    gl_Position = projection * view * rotation * aPos;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    color = aColor;
}