#version 410 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture1, textureCoord), texture(ourTexture2, textureCoord), 0.2);
}