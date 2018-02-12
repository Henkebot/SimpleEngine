#version 440
layout(location = 0) out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;
uniform sampler2D specularTexture;
uniform sampler2D bumpMap;

uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform float lightPower;

in DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec3 normals;

} fs_in;

void main()
{
	vec3 LightColor = vec3(1,1,1);

	vec3 MaterialDiffuseColor = texture(diffuseTexture, fs_in.uvs).rgb * diffuseColor;
	vec3 MaterialAmbientColor = texture(ambientTexture, fs_in.uvs).rgb;
	vec3 MaterialSpecularColor = texture(specularTexture, fs_in.uvs).rgb;

	vec3 normal = texture(bumpMap, fs_in.uvs).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	// Diffuse
	vec3 lightDir = normalize(fs_in.TangentFragPos - fs_in.TangentLightPos);
	float diff = clamp(dot(-lightDir,normal), 0, 1);
	vec3 diffuseFinal = diff * MaterialDiffuseColor;
	
	// Specular
	vec3 viewDir = normalize(fs_in.TangentFragPos - fs_in.TangentViewPos );
	vec3 reflectDir = reflect(lightDir, normal);
	vec3 halfwayDir = normalize(lightDir - viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 finalSpecular = vec3(1) * spec;

	float distance = length(fs_in.TangentFragPos - fs_in.TangentLightPos);
	outColor = vec4(fs_in.TangentLightPos, 1);
	outColor = vec4(MaterialAmbientColor + 
					(diffuseFinal * LightColor * lightPower / (distance * distance)) + 
					(finalSpecular * LightColor * lightPower / (distance * distance)),1.0);
}
