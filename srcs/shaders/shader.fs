#version 330 core

uniform sampler2D texture1;
uniform float aMixValue;

in vec2 TexCoord;
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(color, 1.0f), texture(texture1, TexCoord), aMixValue);
}