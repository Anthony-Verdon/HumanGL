#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 aColor;
uniform float aMixValue;

void main()
{
    FragColor = mix(vec4(aColor, 1.0f), texture(texture1, TexCoord), aMixValue);
}