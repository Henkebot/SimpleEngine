#version 440
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normals;
layout(location = 3) in vec3 in_tangents;
layout(location = 4) in vec3 in_bitangents;


uniform mat4 Projection, View, World;
uniform vec3 Light_pos, Camera_Pos;

out DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec3 normals;

} vs_out;


void main()
{
	vs_out.uvs = in_uvs;
	vs_out.pos = (World * vec4(in_pos,1.0f)).xyz;
	vs_out.normals = in_normals;
	
	mat3 normalMatrix = transpose(inverse(mat3(World)));
	vec3 T = normalize(normalMatrix * in_tangents);
	vec3 N = normalize(normalMatrix * in_normals);
	T = normalize(T - dot(T,N) * N);
	vec3 B = cross(N,T);
	
	mat3 TBN = transpose(mat3(T,B,N));
	vs_out.TangentLightPos = TBN * Light_pos;
	vs_out.TangentViewPos = TBN * Camera_Pos;
	vs_out.TangentFragPos = TBN * vs_out.pos;

	gl_Position = Projection * View * vec4(vs_out.pos,1.0f);
}