#version 460 core

out vec4 FragColor;

void main()
{
   FragColor = vec4(float(gl_FragCoord.x)/ 1000.0, float(gl_FragCoord.y) / 1000.0,0.0, 1.0);
}