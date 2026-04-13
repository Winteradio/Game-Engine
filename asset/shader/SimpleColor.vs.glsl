#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(binding = 0) uniform CameraData
{
    mat4 view;
    mat4 projection;

    vec3 cameraPosition;
    float nearPlane;
    float farPlane;
    float fov;
};

layout(binding = 1) buffer InstanceData
{
    mat4 transforms[];
};

void main()
{
    mat4 transform = transforms[max(gl_InstanceID, 0)];
    gl_Position = projection * view * transform * vec4(position, 1.0);
}