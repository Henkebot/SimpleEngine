#version 440
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normals;

out DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 normals;
} vs_out;

uniform mat4 Projection, View, World;

void main()
{
	vs_out.uvs = in_uvs;
	vs_out.normals = in_normals;
	vs_out.pos = (World * vec4(in_pos,1.0f)).xyz;

	gl_Position = Projection * View * vec4(vs_out.pos,1.0f);
}