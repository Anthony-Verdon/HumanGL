#version 330 core

in vec3 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform float mixValue;
uniform vec3 color;

void main()
{
    FragColor = mix(vec4(color, 1.0f), texture(texture1,  vec2(TexCoord.x, TexCoord.y)), mixValue);
}