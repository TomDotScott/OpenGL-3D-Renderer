#version 330 core

out vec4 FragColor;

in vec3 colour;

in vec2 texCoord;

in vec3 Normal;
in vec3 curPos;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
    // the degrees of the cone for the spotlight
    // Two cones means that we'll get a gradient from light to dark
    float outerCone = cos(radians(25.0f));
    float innerCone = cos(radians(20.0f));

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


    // In order to use the cone, find the angle between the light and the part being lit
    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);

    // Find the intensity at the current spot
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    FragColor = texture(diffuse0, texCoord) * lightColour * (diffuse * intensity + ambient + specular * intensity) + texture(specular0, texCoord).r * specular * intensity;
}