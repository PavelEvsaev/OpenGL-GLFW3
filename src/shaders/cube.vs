#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);

    FragPos = vec3(modelMatrix * vec4(position, 1.0f));
    Normal = normalize(normalMatrix * normal);
    TexCoords = texCoords;
}