#version 330 core

uniform sampler2D image;
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

void main()
{
    FragColor = texture(image, TexCoord);
}