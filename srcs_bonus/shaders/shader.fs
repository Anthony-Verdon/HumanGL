#version 330 core

in vec2 texCoord;
in vec3 color;
in float mixValue;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = mix(vec4(color, 1.0f), texture(texture1, texCoord), mixValue);
}