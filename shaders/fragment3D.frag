#version 440
#pragma optimize(off)

layout(location = 0) out vec4 outColor;

in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
	vec4 normal;
} fs_in;

void main()
{
	float intensity = abs(3.0/length(fs_in.position.xyz - fs_in.lightPos.xyz));
	

	vec3 finalColor = clamp(fs_in.color.rgb * intensity, 0.0f, 1.0f);
	vec3 normalColor = abs(fs_in.normal.xyz);
	outColor = vec4(normalColor,1.0f);
}