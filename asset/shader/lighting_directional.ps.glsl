#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGDepth;
layout(binding = 1) uniform sampler2D tGNormal;
layout(binding = 2) uniform sampler2D tGAlbedo;
layout(binding = 3) uniform sampler2D tGParam;

layout(std430, binding = 0) readonly buffer uLight
{
	vec3 color;
	vec3 position;
	float intensity;
	vec3 direction;
} light;

layout(std140, binding = 1) uniform uCamera
{
	mat4 view;
	mat4 proj;
	mat4 invView;
	mat4 invProj;
	vec3 position;
	vec3 direction;
} camera;

in vec2 outTexCoord;

const float PI = 3.14159265359;

float D_GGX(float NdotH, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;
    float d  = NdotH * NdotH * (a2 - 1.0) + 1.0;
	return a2 / max(PI * d * d, 0.0001);
}

float G_Schlick(float NdotX, float roughness)
{
    float k = (roughness + 1.0);
    k = (k * k) / 8.0;
    return NdotX / (NdotX * (1.0 - k) + k);
}

vec3 F_Schlick(float HdotV, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - HdotV, 0.0, 1.0), 5.0);
}

float G_Smith(float NdotV, float NdotL, float roughness)
{
    return G_Schlick(NdotV, roughness) * G_Schlick(NdotL, roughness);
}

void main()
{
    vec2 uv = outTexCoord;

    float depth = texture(tGDepth, uv).r;
    if (depth >= 1.0)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec4 ndcPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = camera.invProj * ndcPos;
    viewPos /= viewPos.w;
    vec3 position = (camera.invView * viewPos).xyz;
	vec3 normal = normalize(texture(tGNormal, uv).rgb * 2.0 - 1.0);
    vec3 albedo = texture(tGAlbedo, uv).rgb;

	float roughness = texture(tGParam, outTexCoord).r;
	float metallic = texture(tGParam, outTexCoord).g;
	float ambientOcclusion = texture(tGParam, outTexCoord).b;

	vec3 viewDir = normalize(camera.position - position);
	vec3 lightDir = normalize(-light.direction);
	vec3 halfDir = normalize(viewDir + lightDir);

    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0001);
    float NdotH = max(dot(normal, halfDir), 0.0);
    float HdotV = max(dot(halfDir, viewDir), 0.0);

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // DFG
    float D = D_GGX(NdotH, roughness);
    float G = G_Smith(NdotV, NdotL, roughness);
    vec3  F = F_Schlick(HdotV, F0);

    // Specular lobe
    vec3 specular = (D * G * F) / max(4.0 * NdotV * NdotL, 0.0001);

    // Diffuse lobe
    vec3 kD      = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 diffuse = kD * albedo / PI;

    vec3 lightColor = light.color * light.intensity;
    vec3 Lo         = (diffuse + specular) * lightColor * NdotL;
    vec3 ambient    = albedo * 0.3 * ambientOcclusion;

    FragColor = vec4(ambient + Lo, 1.0);
}