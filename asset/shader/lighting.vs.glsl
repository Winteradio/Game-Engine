#version 460 core

layout(location = 0) in vec2 position;
layout(location = 6) in vec2 texcoord;

layout(location = 1) out vec2 outTexCoord;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);

	outTexCoord = texcoord;
}
