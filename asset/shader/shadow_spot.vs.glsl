#version 460 core

#include "Common.glsl"

DECLARE_LIGHT_PASS

layout(location = 0) in vec3 position;

DECLARE_TRANSFORM

void main()
{
	mat4 model = transform.datas[RHI_InstanceIndex];
	gl_Position = uLightProj * uLightView * model * vec4(position, 1.0);
}
