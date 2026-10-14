#version 460 core

#include "Common.glsl"

layout(location = 0) out vec4 tGNormal;
layout(location = 1) out vec4 tGAlbedo;
layout(location = 2) out uvec4 tGPhong;

layout(binding = BINDING_DIFFUSE) uniform sampler2D tDiffuse;

DECLARE_MATERIAL_PHONG

layout(location = 0) in vec3 outNormal;
layout(location = 1) in vec2 outTexCoord;

void main()
{
	tGNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
	tGAlbedo = texture(tDiffuse, outTexCoord) * vec4(material.diffuse, material.opacity);

	float encode = 255.0;
	uvec3 spec = uvec3(
		uint(material.specular.r * float(encode)),
		uint(material.specular.g * float(encode)),
		uint(material.specular.b * float(encode))
	);
	uvec3 emis = uvec3(
		uint(material.emissive.r * float(encode)),
		uint(material.emissive.g * float(encode)),
		uint(material.emissive.b * float(encode))
	);

	tGPhong = uvec4(
		spec.r | (emis.r << 8u),
		spec.g | (emis.g << 8u),
		spec.b | (emis.b << 8u),
		uint(material.shininess)
	);
}
