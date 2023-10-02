#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
out vec2 texCoord;
out vec3 aFragColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    texCoord = vec2(aPos.x, aPos.y);
    aFragColor = color;
}