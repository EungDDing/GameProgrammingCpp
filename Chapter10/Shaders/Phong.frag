#version 330

// input from vertex shader
// tex coord input from vertex shader
in vec2 fragTexCoord;
// normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// this correspond to the output color to the color buffer
out vec4 outColor;

// this is used for the texture sampling
uniform sampler2D uTexture;

// create a struct for directional light
struct DirectionalLight 
{
    // Direction of light
    vec3 mDirection;
    // Diffuse color
    vec3 mDiffuseColor;
    // Specular color
    vec3 mSpecColor;    
};

// uniforms for lighting
// camera position (in world space)
uniform vec3 uCameraPos;
// specular power for this surface
uniform float uSpecPower;
// ambient light level
uniform vec3 uAmbientLight;

// Direction Light
uniform DirectionalLight uDirLight;

void main() 
{
    // surface normal
    vec3 N = normalize(fragNormal);
    // vector from surface to light
    vec3 L = normalize(-uDirLight.mDirection);
    // vector from surface to camera
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // reflection of -L about N
    vec3 R = normalize(reflect(-L, N));

    // compute phong reflection
    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);
    if (NdotL > 0) 
    {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += Diffuse + Specular;
    }

    // final color is texture color thimes phong light(alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}