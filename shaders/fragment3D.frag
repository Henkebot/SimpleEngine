#version 440
#pragma optimize(off)

layout(location = 0) out vec4 outColor;

uniform vec3 ViewPos;

in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
	vec4 normal;
} fs_in;

void main()
{
	float specularStrength = 0.5;

	float ambientStrength = 0.5f;
	vec3 ambient = vec3(1.0, 1.0,1.0) * ambientStrength;

	vec3 lightUnitVector = normalize(fs_in.lightPos.xyz - fs_in.position.xyz);

	vec3 viewDir = normalize(ViewPos - fs_in.position.xyz);
	vec3 reflectDir = reflect(-lightUnitVector, fs_in.normal.xyz);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	vec3 specular = vec3(1.0, 1.0, 1.0) * specularStrength * spec;

	float diffuse = max(dot(fs_in.normal.xyz, lightUnitVector),0.0);
	float intensity = 1.0/length(fs_in.position.xyz - fs_in.lightPos.xyz);

	vec3 finalColor = clamp(fs_in.color.rgb * (ambient + diffuse + specular) * intensity, 0.0f, 1.0f);

	outColor = vec4(finalColor,1.0f);
}