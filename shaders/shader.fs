#version 330 core

uniform sampler2D texture1;
uniform float aMixValue;
uniform vec3 lightColor;

in vec2 TexCoord;
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(lightColor * color, 1.0f), texture(texture1, TexCoord), aMixValue);
}