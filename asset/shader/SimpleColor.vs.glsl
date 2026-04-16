#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(binding = 0) uniform CameraData
{
    mat4 viewMatrix;
    mat4 projMatrix;

    vec3 cameraPosition;
};

layout(binding = 1) buffer InstanceData
{
    mat4 transforms[];
};

out vec3 outNormal;

void main()
{
    mat4 transform = transforms[max(gl_InstanceID, 0)];

    gl_Position = projMatrix * viewMatrix * transform * vec4(position, 1.0);

    mat3 affin = mat3(transform);
    float affinDet = determinant(affin);
    float valid = step(1e-6, abs(affinDet));

    mat3 validAffin = affin * valid + mat3(1.0 - valid);
    mat3 normalTransform = transpose(inverse(validAffin));

    outNormal = normalize(normalTransform * normal);
}