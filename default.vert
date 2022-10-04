#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec2 aTexCoord;

// The colour for the fragment shader
out vec3 colour;

// UVs of the texture
out vec2 texCoord;

// Scale of the vertices
uniform float scale;

// The various matrices to give perspective
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);

    colour = aColour;

    texCoord = aTexCoord;
}