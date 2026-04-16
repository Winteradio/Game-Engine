#version 460 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D albedo;

uniform vec4 tintColor;

in vec3 outNormal;

void main()
{
    FragColor = vec4(outNormal * 0.5 + 0.5, 1.0);
}