#version 450 core

layout(location = 0) out vec4 tPosition;
layout(location = 1) out vec4 tNormal;
layout(location = 2) out vec4 tAlbedo;

out vec4 FragColor;

in vec3 outNormal;
in vec3 outPosition;

void main()
{
    FragColor = vec4(outNormal * 0.5 + 0.5, 1.0);

    tPosition = vec4(outPosition, 1.0);
    tNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
    tAlbedo = vec4(1.0, 1.0, 1.0, 1.0);
}