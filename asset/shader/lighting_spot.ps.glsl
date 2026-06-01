#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGDepth;
layout(binding = 1) uniform sampler2D tGNormal;
layout(binding = 2) uniform sampler2D tGAlbedo;
layout(binding = 3) uniform usampler2D tGPhong;

layout(std430, binding = 0) readonly buffer uLight
{
	vec3 color;
	vec3 position;
	float intensity;
    vec3 direction;
    float range;
    float innerAngle;
    float outerAngle;
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
    uvec4 phong = texture(tGPhong, uv);

    vec3 diffuse = albedo.rgb;
    vec3 specular = vec3(
        float(phong.r & 0xFF) / 255.0,
        float(phong.g & 0xFF) / 255.0,
        float(phong.b & 0xFF) / 255.0
        );

    vec3 emissive = vec3(
        float((phong.r >> 8u) & 0xFF) / 255.0,
        float((phong.g >> 8u) & 0xFF) / 255.0,
        float((phong.b >> 8u) & 0xFF) / 255.0
        );

    float opacity = albedo.a;
    float shininess = float(phong.a);

    vec3 position = GetWorldPosition(uv);
    vec3 normal = normalize(texture(tGNormal, uv).rgb * 2.0 - 1.0);

    vec3 rayDir  = normalize(position - light.position);
    vec3 viewDir = normalize(camera.position - position);

	vec3 incidentDir = rayDir - 2.0 * max(dot(normal, rayDir), 0.0) * normal;
	vec3 reflectDir = reflect(incidentDir, normal);
	vec3 halfDir = normalize(viewDir - incidentDir);

	float NdotL = dot(normal, -incidentDir);
    float RdotH = max(dot(reflectDir, halfDir), 0.0);

	float cosInner = cos(radians(light.innerAngle));
	float cosOuter = cos(radians(light.outerAngle));
	float cosTheta = dot(rayDir, light.direction);

	float epsilon = cosInner - cosOuter;
	float intensity = clamp((cosTheta - cosOuter) / epsilon, 0.0, 1.0) * light.intensity;

    float lightDist = length(light.position - position);
    float distRatio = lightDist / max(light.range, 0.0001);
    float falloff     = clamp(1.0 - distRatio * distRatio, 0.0, 1.0);
    float attenuation = falloff / (lightDist * lightDist + 1.0);

    vec3 lightOut = (diffuse * abs(NdotL) + specular * pow(RdotH, shininess)) * light.color * intensity * attenuation;
    lightOut += emissive;

    FragColor = vec4(lightOut, opacity);
}