#version 440
#pragma optimize(off)

layout(location = 0) out vec4 outColor;

in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
} fs_in;

void main()
{
	float intensity = 1.0/length(fs_in.position.xyz - fs_in.lightPos.xyz);

	vec3 finalColor = clamp(fs_in.color.rgb * intensity, 0.0f, 1.0f);
	outColor = vec4(finalColor,1.0f);
}