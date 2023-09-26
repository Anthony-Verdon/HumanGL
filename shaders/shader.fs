#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixPercentage;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixPercentage);
    FragColor = vec4(0.92, 0.41, 0.0, 1.0);
}