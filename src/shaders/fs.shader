#version 450 core

out vec4 FragColor;

in vec3 vNormal;
in vec3 fragPos;

struct Model {
    vec4 color;
};

uniform Model material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 diffuse = vec3(material.color);
    vec3 ambient = vec3(0.0f);
    vec3 specular = vec3(0.2f) * diffuse;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0f);

    ambient = 0.3f * diffuse;
    diffuse = diff * diffuse;
    specular = spec * specular;

    diffuse = diff * diffuse;
    specular = spec * specular;

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
};