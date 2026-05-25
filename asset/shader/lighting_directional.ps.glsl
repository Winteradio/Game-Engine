#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGNormal;
layout(binding = 1) uniform sampler2D tGAlbedo;
layout(binding = 2) uniform sampler2D tGParam;

layout(std430, binding = 0) readonly buffer uLight
{
	vec3 color;
	vec3 position;
	float intensity;
	vec3 direction;
} light;

in vec2 outTexCoord;

void main()
{
	float roughness = texture(tGParam, outTexCoord).r;
	float metallic = texture(tGParam, outTexCoord).g;
	float ambientOcclusion = texture(tGParam, outTexCoord).b;

	vec3 albedo = texture(tGAlbedo, outTexCoord).rgb;
	vec3 normal = texture(tGNormal, outTexCoord).xyz;
	normal = 2.0 * normal - 1.0;

	float lightFactor = dot(-light.direction, vec3(normal));

	FragColor = vec4(albedo * light.color * light.intensity * lightFactor, 1.0);
	FragColor = normalize(FragColor);
}