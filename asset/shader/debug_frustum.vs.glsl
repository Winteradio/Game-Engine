#version 460 core

#include "Common.glsl"

DECLARE_CAMERA
DECLARE_LIGHT_PASS

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 outNormal;

void main()
{
	outNormal = transpose(inverse(mat3(camera.view))) * normal;

	vec4 ndcPos = vec4(position, 1.0);
	vec4 viewPos = inverse(uLightProj) * ndcPos;
	viewPos /= viewPos.w;

	vec4 worldPos = inverse(uLightView) * viewPos;

	gl_Position = camera.proj * camera.view * worldPos;
}
