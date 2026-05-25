#version 450 core

out vec4 FragColor;

layout(location = 0) out vec4 tGNormal;
layout(location = 1) out vec4 tGAlbedo;
layout(location = 2) out vec4 tGParam;

layout(binding = 0) uniform sampler2D tDiffuse;

layout(std430, binding = 3) readonly buffer uMaterial
{
	vec3 baseColor;
	float roughness;
	float metallic;
	float ambientOcclusion;
	float opacity;
} material;

in vec2 outTexCoord;
in vec3 outNormal;

void main()
{
	tGNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
	tGAlbedo = texture(tDiffuse, outTexCoord);
	tGParam = vec4(material.roughness, material.metallic, material.ambientOcclusion, 0.0);

	FragColor = tGAlbedo;
}