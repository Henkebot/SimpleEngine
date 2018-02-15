#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_uvs;
layout (location = 2) in vec3 in_normals;
layout (location = 3) in vec3 in_tangents;
layout (location = 4) in vec3 in_bitangents;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
uniform vec3 Light_Pos;
uniform vec3 Camera_Pos;

void main()
{
    vs_out.FragPos = vec3(World * vec4(in_pos, 1.0));
    vs_out.TexCoords = in_uvs;
    vs_out.Normal = in_normals;
	
	mat3 normalMatrix = transpose(inverse(mat3(World)));
	vec3 T = normalize(normalMatrix * in_tangents);
	vec3 N = normalize(normalMatrix * in_normals);
	T = normalize(T - dot(T,N) * N);
	vec3 B = cross(N,T);

	mat3 TBN = transpose(mat3(T,B,N));
	vs_out.TangentLightPos = TBN * Light_Pos;
	vs_out.TangentViewPos = TBN * Camera_Pos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
   
    gl_Position = Projection * View * vec4( vs_out.FragPos, 1.0);
}