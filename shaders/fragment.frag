
#version 440
#pragma optimize(off)

layout (location = 0) out vec4 outColor;
uniform vec3 ambientColor, diffuseColor, specularColor
unfiorm flaot specularExponent, transparancy, 

/* Things needed for specular lightning:
NO BUMP ATM
vec3:
	ambientColor
	specularColor
	diffuseColor
float:
	specularExponent
texture
	diffuseTexture


*/
in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
} fs_in;


void main()
{
	float intensity = 1.0 / length(fs_in.position.xy - fs_in.lightPos.xy);
	vec3 finalColor = fs_in.color.rgb * intensity;
	outColor = vec4(finalColor,1.0);
	//outColor = fs_in.position;
}