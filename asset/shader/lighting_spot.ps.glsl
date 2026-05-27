#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGDepth;
layout(binding = 1) uniform sampler2D tGNormal;
layout(binding = 2) uniform sampler2D tGAlbedo;
layout(binding = 3) uniform sampler2D tGPhong;

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

    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir  = normalize(camera.position - position);
    vec3 halfDir  = normalize(lightDir + viewDir);

    float NdotL = max(dot(normal, lightDir), 0.0);
    float NdotH = max(dot(normal, halfDir), 0.0);

    vec3 diffuse  = albedo * NdotL / PI;
    vec3 specular = vec3(pow(NdotH, 32.0)) * NdotL;

    vec3 Lo = (diffuse + specular) * light.color * light.intensity;
    vec3 ambient = albedo * 0.03;

    FragColor = vec4(ambient + Lo, 1.0);
}