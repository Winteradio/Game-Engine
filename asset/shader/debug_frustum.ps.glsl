#version 460 core

layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 outNormal;

void main()
{
	FragColor = vec4(outNormal, 0.1);
}
