#version 450 core

layout(location = 0) in vec3 position;
layout(location = 5) in vec2 texcoord;

layout(binding = 0) uniform uCamera
{
    mat4 viewMatrix;
    mat4 projMatrix;

    vec3 cameraPosition;
    vec3 cameraDirection;
};

layout(binding = 1) buffer uTransform
{
    mat4 transforms[];
};

out vec2 outTexCoord;

void main()
{
    mat4 transform = transforms[max(gl_InstanceID, 0)];

    gl_Position = projMatrix * viewMatrix * transform * vec4(position, 1.0);
    
    outTexCoord = texcoord;
}