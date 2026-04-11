#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

void main()
{
   vec4 vPos = vec4(position.x, position.y, position.z, 1.0);

   gl_Position = vPos;
}