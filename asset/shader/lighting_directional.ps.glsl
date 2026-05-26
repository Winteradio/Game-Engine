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

vec3 GetWorldPosition(vec2 uv)
{
    float depth = texture(tGDepth, uv).r;

    vec4 ndcPos = vec4(uv * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    vec4 viewPos = camera.invProj * ndcPos;
    viewPos /= viewPos.w;

    vec3 position = (camera.invView * viewPos).xyz;
    return position;
}

void main()
{
    vec2 uv = outTexCoord;

    float depth = texture(tGDepth, uv).r;
    if (depth >= 1.0) 
    { 
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); return; 
    }

    vec4 albedo = texture(tGAlbedo, uv);
    vec4 phong = texture(tGPhong, uv);

    float opacity = albedo.a;
    vec3 diffuse = albedo.rgb;

    vec3 specular = vec3(
        mod(phong.r, 255.0) / 255.0,
        mod(phong.g, 255.0) / 255.0,
        mod(phong.b, 255.0) / 255.0
        );

    vec3 emissive = vec3(
        floor(phong.r/ 255.0) / 255.0,
        floor(phong.g/ 255.0) / 255.0,
        floor(phong.b/ 255.0) / 255.0
        );

    float shininess = phong.a / 100.0;

    vec3 position = GetWorldPosition(uv);
    vec3 normal = normalize(texture(tGNormal, uv).rgb * 2.0 - 1.0);

    vec3 lightDir = normalize(-light.direction);
    vec3 viewDir = normalize(camera.position - position);

    float NdotL = max(dot(normal, lightDir), 0.0);
    
    vec3 reflection = reflect(-lightDir, normal);
    float RdotV = max(dot(reflection, viewDir), 0.0);

    vec3 lightOut = (diffuse * NdotL + specular * pow(RdotV, shininess)) * light.color * light.intensity;
    lightOut += emissive;

    FragColor = vec4(lightOut, opacity);
}