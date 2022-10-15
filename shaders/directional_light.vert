#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColour;
layout(location = 3) in vec2 aTexCoord;

// The colour for the fragment shader
out vec3 colour;

// UVs of the texture
out vec2 texCoord;

out vec3 Normal;
out vec3 curPos;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    curPos = vec3(modelMatrix * translation * rotation * scale * vec4(aPos, 1));

    gl_Position = camMatrix * vec4(curPos, 1.0);

    colour = aColour;

    // Rotate the UVs by 90 degrees 
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTexCoord;

    Normal = aNormal;
}

