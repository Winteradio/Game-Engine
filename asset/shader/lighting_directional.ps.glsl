#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGDepth;
layout(binding = 1) uniform sampler2D tGNormal;
layout(binding = 2) uniform sampler2D tGAlbedo;
layout(binding = 3) uniform usampler2D tGPhong;
layout(binding = 4) uniform sampler2D tGShadow;

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

uniform mat4 uLightView;
uniform mat4 uLightProj;

in vec2 outTexCoord;

const float PI = 3.14159265359;
const float EPSILON = 1e-6;
const float GRAZING_ANGLE = PI * (90.0 - 1.0) / 180;
const float GRAZING_COS = cos(GRAZING_ANGLE);
const float GRAZING_SIN = sqrt(max(0.0, 1.0 - GRAZING_COS * GRAZING_COS));
const float GRAZING_TAN = GRAZING_SIN / GRAZING_COS;

float GetShadowFactor(vec3 worldPos, vec3 worldNor)
{
    vec2 textureRect = vec2(textureSize(tGShadow, 0));
    vec2 texelRect = 1.0 / textureRect;

    vec4 clipPos = uLightProj * uLightView * vec4(worldPos, 1.0);
    vec3 ndcPos = clipPos.xyz / (sign(clipPos.w) * max(clipPos.w, EPSILON));
    vec2 screenPos = clamp(ndcPos.xy * 0.5 + 0.5, vec2(0.0), vec2(1.0));
    float posDepth = clamp(ndcPos.z * 0.5 + 0.5, 0.0, 1.0);
    float shadowDepth = texture(tGShadow, screenPos).r;

    float cosTheta = dot(worldNor, -normalize(light.direction));
    float sinTheta = sqrt(max(0.0, 1.0 - cosTheta * cosTheta));
    float tanTheta = sinTheta / max(cosTheta, EPSILON);
    tanTheta = min(tanTheta, GRAZING_TAN);

    float orthoWidth = 2.0 / uLightProj[0][0];
    float orthoHeight = 2.0 / uLightProj[1][1];
    float worldTexel = max(orthoWidth * texelRect.x, orthoHeight * texelRect.y);

    float depthRange = abs(2.0 / uLightProj[2][2]);
    float bias = max(worldTexel * tanTheta / depthRange, EPSILON);


    float shadowFactor = step(bias, abs(posDepth - shadowDepth)) * step(shadowDepth, posDepth);
    shadowFactor *= step(0.0, clipPos.w);
    return shadowFactor;
}

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

    vec3 incidentDir = normalize(light.direction);
    vec3 viewDir = normalize(camera.position - position);
    vec3 halfDir = normalize(viewDir - incidentDir);

    vec3 reflectDir = reflect(incidentDir, normal);

    float NdotL = max(dot(normal, -incidentDir), 0.0);
    float RdotH = max(dot(reflectDir, halfDir), 0.0);

    vec3 lightOut = (diffuse * NdotL + specular * pow(RdotH, shininess)) * light.color * light.intensity;
    lightOut += emissive;

    vec3 ambient = albedo.rgb * 0.03;

    float shadowFactor = GetShadowFactor(position, normal);

    FragColor = vec4(lightOut * (1.0 - shadowFactor) + ambient, opacity);
}