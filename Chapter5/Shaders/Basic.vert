#version 330

// this should correspond to the data stored
// for each vertex in the vertex buffer.
in vec3 inPosition;

void main() 
{
    // the vertex shader needs to output a 4D coordinate
    // for now set the 4th coordinate to 1.0
    gl_Position = vec4(inPosition, 1.0);
}