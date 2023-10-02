#version 330 core

in vec2 texCoord;
in vec3 aFragColor;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = mix(vec4(aFragColor, 1.0f), texture(texture1, texCoord), 0);
}