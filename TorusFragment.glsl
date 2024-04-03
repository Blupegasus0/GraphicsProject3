#version 120

// Interpolated values from the vertex shaders
varying vec2 UV;
varying vec3 Position;

// Values that stay constant for the whole mesh.
uniform sampler2D starTexture;

void main()
    {
            gl_FragColor = texture2D( starTexture, UV );
       
    }
