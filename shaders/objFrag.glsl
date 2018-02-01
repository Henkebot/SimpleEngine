#version 440
layout(location = 0) out vec4 outColor;

uniform sampler2D texture1;

in DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 normals;
} fs_in;

void main()
{
	vec3 lightPos = vec3(0,20,10);

	vec3 lightUnitVector = normalize(lightPos - fs_in.pos);
	vec3 normal = normalize(fs_in.normals);

	float diffuse = max(dot(lightUnitVector,normal),0.0);

	vec4 color = texture(texture1, fs_in.uvs);

	outColor = color * diffuse;
}
