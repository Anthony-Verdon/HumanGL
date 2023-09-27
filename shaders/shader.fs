#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixPercentage;
void main()
{   
    FragColor = texture(texture1, texCoord);
}