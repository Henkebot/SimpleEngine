#version 440

layout(location = 0) out vec4 outColor;

in DATA
{
	vec4 pos;
	vec4 color;
	vec4 lightPos;
} fs_in;

void main()
{
	float intensity = 1.0/length(fs_in.pos.xyz - fs_in.lightPos.xyz);
	vec3 finalColor = fs_in.color.rgb * intensity;
	outColor = vec4(finalColor,1.0f);
}