#version 450 core

layout(location = 0) in vec3 position;

layout(std430, binding = 0) readonly buffer uTransform
{
	mat4 datas[];
} transform;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
    mat4 transform = transform.datas[max(gl_InstanceID, 0)];
    gl_Position = projMatrix * viewMatrix * transform * vec4(position, 1.0);
}