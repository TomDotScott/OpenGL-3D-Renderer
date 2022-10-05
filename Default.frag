#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

in vec3 Normal;
in vec3 curPos;

uniform sampler2D tex0;

// For Flat Shading
uniform vec4 lightColour;
// For Diffuse Shading
uniform vec3 lightPos;
// For Specular Highlighting
uniform vec3 camPos;

void main()
{
    // Add some ambient lighting
    float ambient = 0.2f;

    // Find the direction that light is pointing
    vec3 normal = normalize(Normal);

    vec3 lightDirection = normalize(lightPos - curPos);

    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - curPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 8);

    float specular = specularLight * specAmount;


    FragColor = texture(tex0, texCoord) * lightColour * (diffuse + ambient + specular);
}