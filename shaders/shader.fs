#version 330 core

uniform sampler2D texture1;
uniform float aMixValue;
uniform vec3 aLightColor;

in vec2 TexCoord;
in vec3 color;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(aLightColor * color, 1.0f), texture(texture1, TexCoord), aMixValue);
}