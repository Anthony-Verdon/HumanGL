#version 330 core

uniform sampler2D image;
uniform bool useTexCoord;
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

void main()
{
    if (useTexCoord)
        FragColor = texture(image, TexCoord);
    else
        FragColor = vec4(1.0f);
}