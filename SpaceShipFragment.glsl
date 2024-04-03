#version 330 core

in vec2 TextureCoords;
out vec4 color;

uniform sampler2D shipTexture;

void main()
{
    color = texture(shipTexture, TextureCoords);
}