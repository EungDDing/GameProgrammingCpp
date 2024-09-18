#version 330

// uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// attribute 0 is position, 1 is tex coords
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// any vertex outputs
out vec2 fragTexCoord;
// normal vector (in world space)
out vec3 fragNormal;
// position (in world space)
out vec3 fragWorldPos;

void main()
{
    // convert position to homogeneous coordinates
    vec4 pos = vec4(inPosition, 1.0);
    // transform position to world space
    pos = pos * uWorldTransform;
    // save world position
    fragWorldPos = pos.xyz;
    // transform to clip space
    gl_Position = pos * uViewProj;

    // transform normal into world space (w = 0)
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

    // pass along the texture coordinate to frag shader
    fragTexCoord = inTexCoord;
}