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

float GetShadowFactor(vec3 worldPos, vec3 worldNor)
{
    vec4 clipPos = uLightProj * uLightView * vec4(worldPos, 1.0);
    vec4 ndcPos = mix(clipPos, clipPos/clipPos.w, step(1e-6, abs(clipPos.w)));

    vec3 projCoords = ndcPos.xyz * 0.5 + 0.5;
	projCoords.z = clamp(projCoords.z, 0.0, 1.0);

    vec2 shadowUV = projCoords.xy;

    vec2 texelSize = textureSize(tGShadow, 0);
    float texelRatio = 1.0 / float(max(texelSize.x, texelSize.y));
    float tangentRatio = length(light.direction - worldNor * dot(light.direction, worldNor));
    
	float shadowBias = texelRatio * tangentRatio;

    float currentDepth = projCoords.z;

    float shadowFactor = 0.0;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            vec2 pcfUV = shadowUV + vec2(x,y) / texelSize;
            float pcfDepth = texture(tGShadow, pcfUV).r;
            
            shadowFactor += step(pcfDepth, currentDepth - shadowBias);
        }
    }

    shadowFactor /= 9.0;

    float fadeMargin = 0.1; 
    
    float distToEdgeX = min(projCoords.x, 1.0 - projCoords.x);
    float distToEdgeY = min(projCoords.y, 1.0 - projCoords.y);
    float distToEdgeZ = abs(1.0 - projCoords.z);
    
    float minFade = min(min(distToEdgeX, distToEdgeY), distToEdgeZ);

	float fadeFactor = mix(1.0, smoothstep(0.0, fadeMargin, minFade), step(1e-6, fadeMargin - minFade));

    return shadowFactor * fadeFactor;
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