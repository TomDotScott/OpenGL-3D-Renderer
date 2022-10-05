#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

// The colour for the fragment shader
out vec3 colour;

// UVs of the texture
out vec2 texCoord;

out vec3 Normal;
out vec3 curPos;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
    curPos = vec3(modelMatrix * vec4(aPos, 1));

    gl_Position = camMatrix * vec4(curPos, 1.0);

    colour = aColour;

    texCoord = aTexCoord;
    Normal = aNormal;
}