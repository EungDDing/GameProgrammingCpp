#version 330

// uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// attribute 0 is position, 1 is tex coords
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

// add texture coordinate as output
out vec2 fragTexCoord;

void main()
{
    // convert position to homogeneous coordinates
    vec4 pos = vec4(inPosition, 1.0);
    // transform position to world space, then clip space
    gl_Position = pos * uWorldTransform * uViewProj;

    // transform
    // pass along the texture coordinate to frag shader
    fragTexCoord = inTexCoord;
}