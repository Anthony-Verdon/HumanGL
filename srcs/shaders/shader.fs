//fragment shader: for the color/texture

#version 330 core


uniform sampler2D texture1;
uniform vec3 aColor;
uniform float aMixValue;

in vec2 texCoord;
in vec3 Color2;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(Color2, 1.0f), texture(texture1, texCoord), aMixValue);
}