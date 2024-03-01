//fragment shader: for the color/texture

#version 330 core


uniform sampler2D texture1;
uniform float aMixValue;

in vec3 color;
in vec2 texCoord;
out vec4 FragColor;

void main()
{
    FragColor = mix(vec4(color, 1.0f), texture(texture1, texCoord), aMixValue);
}
