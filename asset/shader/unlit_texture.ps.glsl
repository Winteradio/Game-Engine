#version 450 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D uDiffuseMap;

in vec2 outTexCoord;

void main()
{
    FragColor = texture(uDiffuseMap, outTexCoord);
}