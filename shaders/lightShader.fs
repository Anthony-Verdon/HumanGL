#version 330 core
out vec4 FragColor;

uniform vec3 aLightColor;

void main()
{
    FragColor = vec4(aLightColor, 1.0);
}