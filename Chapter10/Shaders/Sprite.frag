#version 330

// tex coord input from vertex shader
in vec2 fragTexCoord;

// this correspond to the output color to the color buffer
out vec4 outColor;

// this is used for the texture sampling
uniform sampler2D uTexture;

void main() 
{
    // RGBA of 100% blue, 100% opaque
    outColor = texture(uTexture, fragTexCoord);
}