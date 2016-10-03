#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 LightingColor;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    vec3 Position = vec3(model * vec4(position, 1.0f));
    vec3 Normal = mat3(transpose(inverse(model))) * normal;

	vec3 lightDir = normalize(lightPos - Position);
	float diff = max(dot(Normal, lightDir), 0.0);

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = reflect(-lightDir, Normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

    LightingColor = (ambient + diff + specular);
}