#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tGPosition;
layout(binding = 1) uniform sampler2D tGNormal;
layout(binding = 2) uniform sampler2D tGAlbedo;

layout(binding = 3) uniform uLight
{
    vec3 color;
    vec3 direction;
    vec3 position;
    float intensity;
};

in vec2 outTexCoord;

void main()
{
    vec4 position = texture(tGPosition, outTexCoord);
    vec4 albedo = texture(tGAlbedo, outTexCoord);
    vec4 normal = texture(tGNormal, outTexCoord);
    FragColor = albedo + position + normal;
    FragColor = normalize(FragColor);
}