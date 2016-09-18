#version 410 core

in vec4 vertexColor;
in vec2 textureCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, textureCoord);
}