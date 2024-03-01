//vertex shader (.vs): for the vertex position
//layout: get the variable at a specific location in a certain type
//uniform: global variables, allow to pass data to the GPU for the shaders

#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor2;

uniform mat4 rotation;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 Color2;

void main()
{
    gl_Position = projection * view * rotation * aPos;
    texCoord = vec2(aPos.x, aPos.y);
    Color2 = aColor2;
}