#version 450 core

out vec4 FragColor;
in vec3 outNormal;

void main()
{
    FragColor = vec4(outNormal * 0.5 + 0.5, 1.0);
}