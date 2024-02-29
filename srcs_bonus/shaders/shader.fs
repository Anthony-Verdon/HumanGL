#version 330 core

uniform sampler2D texture1;
uniform vec3 aColor;
uniform float aMixValue;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(aColor, 1.0f), texture(texture1, TexCoord), aMixValue);
}