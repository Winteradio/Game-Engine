#version 450 core

layout(location = 0) out vec4 tGNormal;
layout(location = 1) out vec4 tGAlbedo;
layout(location = 2) out uvec4 tGPhong;

layout(binding = 0) uniform sampler2D tDiffuse;

layout(std430, binding = 3) readonly buffer uMaterial
{
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;
	float shininess;
	float opacity;
} material;

in vec2 outTexCoord;
in vec3 outNormal;

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