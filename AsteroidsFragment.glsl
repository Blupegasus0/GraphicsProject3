#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Position;

// Values that stay constant for the whole mesh.
uniform sampler2D asteroidTexture;

void main()
    {
        gl_FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }