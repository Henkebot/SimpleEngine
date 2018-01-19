#version 440
layout (location = 0) out vec4 outColor;

in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
} fs_in;

void main()
{
	float intensity = 1.0 / length(fs_in.position.xyz - fs_in.lightPos.xyz);
	outColor = fs_in.color * intensity;
}