#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 5) in vec2 texcoord;

layout(std140, binding = 0) uniform uCamera
{
	mat4 view;
	mat4 proj;

	mat4 invView;
	mat4 invProj;

	vec3 camPosition;
	vec3 camDirection;
} camera;

layout(std430, binding = 1) readonly buffer uTransform
{
	mat4 datas[];
} transform;

out vec2 outTexCoord;
out vec3 outNormal;

void main()
{
	mat4 transform = transform.datas[max(gl_InstanceID, 0)];

	gl_Position = camera.proj * camera.view * transform * vec4(position, 1.0);

	mat3 affin = mat3(transform);
	float affinDet = determinant(affin);
	float valid = step(1e-6, abs(affinDet));

	mat3 validAffin = affin * valid + mat3(1.0 - valid);
	mat3 normalTransform = transpose(inverse(validAffin));
	
	outTexCoord = texcoord;
	outNormal = normalize(normalTransform * normal);
}