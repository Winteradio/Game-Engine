#version 450 core

layout(location = 0) in vec2 position;
layout(location = 5) in vec2 texcoord;

out vec2 outTexCoord;
out vec2 outLightPos;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);

	outTexCoord = texcoord;
}