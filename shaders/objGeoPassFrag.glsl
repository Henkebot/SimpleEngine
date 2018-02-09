#version 440
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSpec;


uniform sampler2D bumpMap;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

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
	gPosition = fs_in.pos;
	
	vec3 normal = texture(bumpMap, fs_in.uvs).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	gNormal = fs_in.normals;

	gColorSpec.rgb = texture(diffuseTexture, fs_in.uvs).rgb;

	gColorSpec.a = texture(specularTexture, fs_in.uvs).r;
}


