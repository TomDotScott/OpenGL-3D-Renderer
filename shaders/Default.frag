#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

in vec3 Normal;
in vec3 curPos;

uniform sampler2D albedo0;

// For Flat Shading
uniform vec4 lightColour;

void main()
{
    FragColor = texture(albedo0, texCoord) * lightColour;
}