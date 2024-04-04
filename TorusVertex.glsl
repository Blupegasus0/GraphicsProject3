#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition;
attribute vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;
varying vec3 Position;

// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
    {
        // Output position of the vertex, in clip space : MVP * position
        gl_Position = projection * view * model * vec4(vertexPosition, 1.0f); 
        
        // UV of the vertex.
        UV = vertexUV;
        
        // Position of the vertex.
        Position = vertexPosition;
    }
