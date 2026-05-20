#version 450 core

layout(location = 0) out vec4 tGPosition;
layout(location = 1) out vec4 tGNormal;
layout(location = 2) out vec4 tGAlbedo;

out vec4 FragColor;

layout(binding = 0) uniform sampler2D tDiffuse;

in vec2 outTexCoord;
in vec3 outPosition;
in vec3 outNormal;

void main()
{
    FragColor = texture(tDiffuse, outTexCoord);

    tGPosition = vec4(outPosition, 1.0);
    tGNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
    tGAlbedo = FragColor;
}