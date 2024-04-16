#version 120

uniform sampler2D asteroidTexture;

varying vec2 UV;
varying vec3 Position;

void main()
    {
        gl_FragColor = texture2D( asteroidTexture, UV );
    }