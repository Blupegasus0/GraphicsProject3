#version 330 core

in vec2 TextureCoords;
out vec4 color;

uniform sampler2D Texture;

void main()
{
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}