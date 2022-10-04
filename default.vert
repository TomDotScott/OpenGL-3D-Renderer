#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec2 aTexCoord;

// The colour for the fragment shader
out vec3 colour;

// UVs of the texture
out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * vec4(aPos, 1.0);

    colour = aColour;

    texCoord = aTexCoord;
}