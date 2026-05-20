#version 450 core

layout(location = 0) out vec4 tGPosition;
layout(location = 1) out vec4 tGNormal;
layout(location = 2) out vec4 tGAlbedo;

out vec4 FragColor;

in vec3 outNormal;
in vec3 outPosition;

void main()
{
    FragColor = vec4(outNormal * 0.5 + 0.5, 1.0);

    tGPosition = vec4(outPosition, 1.0);
    tGNormal = vec4(outNormal * 0.5 + 0.5, 1.0);
    tGAlbedo = vec4(1.0, 1.0, 1.0, 1.0);
}