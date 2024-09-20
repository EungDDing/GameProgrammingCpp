#version 330

// this correspond to the output color to the color buffer
out vec4 outColor;

void main() 
{
    // RGBA of 100% blue, 100% opaque
    outColor = vec4(0.0, 0.0, 1.0, 1.0);
}