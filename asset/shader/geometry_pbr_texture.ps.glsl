#version 460 core

#include "Common.glsl"

layout(location = 0) out vec4 tGNormal;
layout(location = 1) out vec4 tGAlbedo;
layout(location = 2) out vec4 tGParam;

layout(binding = BINDING_DIFFUSE) uniform sampler2D tDiffuse;

DECLARE_MATERIAL_PBR

layout(location = 0) in vec3 outNormal;
layout(location = 1) in vec2 outTexCoord;

void main()
{
	tGNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
	tGAlbedo = texture(tDiffuse, outTexCoord);
	tGParam = vec4(material.roughness, material.metallic, material.ambientOcclusion, 0.0);
}
