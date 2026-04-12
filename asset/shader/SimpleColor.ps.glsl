#version 460 core

out vec4 FragColor;

layout(binding = 0) uniform sampler2D albedo;

uniform vec4 tintColor;

void main()
{
    vec4 texColor = texture(albedo, vec2(gl_FragCoord.xy / 1000.0));
    FragColor = texColor * tintColor;
}