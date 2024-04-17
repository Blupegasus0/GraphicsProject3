#version 150 core

varying vec2 UV;
varying vec3 Position;

uniform sampler2D shipTexture;

void main()
{
    gl_FragColor = texture2D(shipTexture, UV);
    //color = vec4(0.0, 1.0, 0.0, 1.0);
}