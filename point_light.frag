#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

in vec3 Normal;
in vec3 curPos;

uniform sampler2D albedo;
uniform sampler2D specularMap;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

float CalculateLightIntensity()
{
    vec3 lightVec = lightPos - curPos;

    float dist = length(lightVec);
    // 1 / d^2 doesn't look that good when simulated
    // Instead, let's use 1 / ad^2 + bd + 1 to be able to control the look
    float a = 3.4f;
    float b = 0.69f;
    return 1.0f / (a * dist * dist + b * dist + 1.0f);
}


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

    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 8 * 10);

    float specular = specularLight * specAmount;

    float intensity = CalculateLightIntensity();

    FragColor = texture(albedo, texCoord) * lightColour * (diffuse + ambient + specular * intensity) + texture(specularMap, texCoord).r * specular * intensity;
}