#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

uniform sampler2D tex0;

uniform vec4 lightColour;

void main()
{
    FragColor = texture(tex0, texCoord) * lightColour;
}