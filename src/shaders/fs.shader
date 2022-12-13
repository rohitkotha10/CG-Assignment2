#version 450 core
// Blinn Phong Shading
out vec4 FragColor;

in vec3 vNormal;
in vec3 fragPos;

struct Model {
    vec4 color;
};

uniform Model material;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 viewPos;

void main() {
    vec3 color = vec3(material.color);
    vec3 ambient = vec3(0.3f) * color;
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 lightDir = normalize(lightPos1 - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0f);

    diffuse += diff * color;
    specular += spec * color * 0.2f;

    lightDir = normalize(lightPos2 - fragPos);
    reflectDir = reflect(-lightDir, norm);
    halfwayDir = normalize(lightDir + viewDir);

    diff = max(dot(norm, lightDir), 0.0f);
    spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0f);

    diffuse += diff * color;
    specular += spec * color * 0.2f;

    diffuse *= 0.5f;
    specular *= 0.5f;

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
};