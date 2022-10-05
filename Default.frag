#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

in vec3 Normal;
in vec3 curPos;

uniform sampler2D tex0;

uniform vec4 lightColour;
uniform vec3 lightPos;

void main()
{
    // Find the direction that light is pointing
    vec3 normal = normalize(Normal);

    vec3 lightDirection = normalize(lightPos - curPos);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    FragColor = texture(tex0, texCoord) * lightColour * diffuse;
}