#version 460 core

#include "Common.glsl"

DECLARE_CAMERA

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

DECLARE_TRANSFORM

layout(location = 0) out vec3 outNormal;

void main()
{
	mat4 model = transform.datas[RHI_InstanceIndex];

	gl_Position = camera.proj * camera.view * model * vec4(position, 1.0);

	mat3 affin = mat3(model);
	float affinDet = determinant(affin);
	float valid = step(1e-6, abs(affinDet));

	mat3 validAffin = affin * valid + mat3(1.0 - valid);
	mat3 normalTransform = transpose(inverse(validAffin));

	outNormal = normalize(normalTransform * normal);
}
