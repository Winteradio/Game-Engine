#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 uLightView;
uniform mat4 uLightProj;

layout(std140, binding = 0) uniform uCamera
{
	mat4 view;
	mat4 proj;

	mat4 invView;
	mat4 invProj;

	vec3 camPosition;
	vec3 camDirection;
} camera;

out vec3 outNormal;

void main()
{
	outNormal = transpose(inverse(mat3(camera.view))) * normal;

	vec4 ndcPos = vec4(position, 1.0);
	vec4 viewPos = inverse(uLightProj) * ndcPos;
	viewPos /= viewPos.w;

	vec4 worldPos = inverse(uLightView) * viewPos;

	gl_Position = camera.proj * camera.view * worldPos;
}