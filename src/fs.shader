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
    vec3 ambient = 0.3f * diffuse;
    vec3 specular = vec3(1.0f);

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f);

    diffuse = diff * diffuse;
    specular = spec * specular;

    vec3 result = (ambient + diffuse + specular) / 3.0f;

    FragColor = vec4(result, 1.0);
};