#version 460 core

#include "Common.glsl"

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct RawTransform
{
	vec3 position;
	vec4 rotation;
	vec3 scale;
};

layout(std430, binding = BINDING_RAW_TRANSFORM) readonly buffer uRawTransform
{
	RawTransform datas[];
} rawTransform;

layout(std430, binding = BINDING_TRANSFORM) buffer uTransform
{
	mat4 datas[];
} transform;

mat4 GetTransform(RawTransform rawTransform)
{
	vec3 position = rawTransform.position;
	vec4 rotation = rawTransform.rotation;
	vec3 scale = rawTransform.scale;

	float qx = rotation.x;
	float qy = rotation.y;
	float qz = rotation.z;
	float qw = rotation.w;

	float x2 = qx * qx;
	float y2 = qy * qy;
	float z2 = qz * qz;

	float xy = qx * qy; float wx = qw * qx;
	float yz = qy * qz; float wy = qw * qy;
	float xz = qx * qz; float wz = qw * qz;

	mat3 scaleMatrix = mat3(
		vec3(scale.x, 0.0, 0.0),
		vec3(0.0, scale.y, 0.0),
		vec3(0.0, 0.0, scale.z)
	);

	mat3 rotationMatrix = mat3(
		vec3(1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy)),
		vec3(2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx)),
		vec3(2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2))
	);

	mat4 affineMatrix = mat4(rotationMatrix * scaleMatrix);
	affineMatrix[3] = vec4(position, 1.0);
	return affineMatrix;
}

void main()
{
	// rawTransform 버퍼 크기가 곧 인스턴스 수 (MeshBatch가 인스턴스 수만큼만 할당함)
	uint uInstanceCount = uint(rawTransform.datas.length());

	uint uThread = gl_GlobalInvocationID.x;
	if (uThread >= uInstanceCount)
	{
		return;
	}

	transform.datas[uThread] = GetTransform(rawTransform.datas[uThread]);
}