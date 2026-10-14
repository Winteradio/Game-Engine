#version 460 core

#include "Common.glsl"

DECLARE_CAMERA
DECLARE_LIGHT_PASS

layout(location = 0) out vec4 FragColor;

layout(binding = BINDING_G_DEPTH) uniform sampler2D tGDepth;
layout(binding = BINDING_G_NORMAL) uniform sampler2D tGNormal;
layout(binding = BINDING_G_ALBEDO) uniform sampler2D tGAlbedo;
layout(binding = BINDING_G_PHONG) uniform usampler2D tGPhong;
layout(binding = BINDING_G_SHADOW) uniform sampler2D tGShadow;

DECLARE_LIGHT_DIRECTIONAL

layout(location = 1) in vec2 outTexCoord;

const float PI = 3.14159265359;
const float EPISLON = 1e-6;
const float GRAZING_ANGLE = PI * (90.0 - 1.0) / 180;
const float GRAZING_COS = cos(GRAZING_ANGLE);
const float GRAZING_SIN = sqrt(max(0.0, 1.0 - GRAZING_COS * GRAZING_COS));
const float GRAZING_TAN = GRAZING_SIN / GRAZING_COS;

float GetShadowFactor(vec3 worldPos, vec3 worldNor)
{
    vec2 textureRect = vec2(textureSize(tGShadow, 0));
    vec2 texelRect = 1.0 / textureRect;

    float cosTheta = dot(worldNor, -normalize(light.direction));
    float sinTheta = sqrt(max(0.0, 1.0 - cosTheta * cosTheta));

	float validFactor = step(GRAZING_COS, cosTheta);

    // --- Normal offset in the world space
    float orthoWidth  = 2.0 / uLightProj[0][0];
    float orthoHeight = 2.0 / uLightProj[1][1];
    float worldTexel  = max(orthoWidth * texelRect.x, orthoHeight * texelRect.y);
    vec3 offsetPos = worldPos + worldNor * worldTexel * sinTheta;

    vec4 clipPos = uLightProj * uLightView * vec4(offsetPos, 1.0);
    vec3 projCoords = clipPos.xyz / clipPos.w * 0.5 + 0.5;
    projCoords.z = clamp(projCoords.z, 0.0, 1.0);

    // --- Receiver plane depth biased on the light space
    vec3 dx = dFdx(projCoords);   // (dudx, dvdx, dzdx)
    vec3 dy = dFdy(projCoords);   // (dudy, dvdy, dzdy)
    float det = dx.x * dy.y - dx.y * dy.x;
	det = mix(EPISLON, det, step(EPISLON, abs(det)));

    vec2 dzduv = vec2(
		(dx.z * dy.y - dy.z * dx.y) / det,  // dz/du
        (dy.z * dx.x - dx.z * dy.x) / det   // dz/dv
    );

	dzduv = clamp(dzduv, -GRAZING_TAN, GRAZING_TAN);

    vec2 shadowUV = projCoords.xy;
    float currentDepth = projCoords.z;
    float constBias = 0.0005;

    float shadowFactor = 0.0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 offset = vec2(x, y) * texelRect;
            vec2 pcfUV = shadowUV + offset;

            float biasedDepth = currentDepth + dot(offset, dzduv);
            float pcfDepth = texture(tGShadow, pcfUV).r;

            shadowFactor += step(pcfDepth, biasedDepth - constBias);
        }
    }

    shadowFactor /= 9.0;

    float fadeMargin = 0.1;

    float distToEdgeX = min(projCoords.x, 1.0 - projCoords.x);
    float distToEdgeY = min(projCoords.y, 1.0 - projCoords.y);
    float distToEdgeZ = abs(1.0 - projCoords.z);

    float minFade = min(min(distToEdgeX, distToEdgeY), distToEdgeZ);
    float fadeFactor = mix(1.0, smoothstep(0.0, fadeMargin, minFade), step(EPISLON, fadeMargin - minFade));

    return shadowFactor * fadeFactor * validFactor;
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