#version 440
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normals;

out vec2 out_uvs;
out vec3 out_normals;
uniform mat4 Projection, View;
void main()
{
	out_uvs = in_uvs;
	out_normals = in_normals;

	gl_Position = Projection * View * vec4(in_pos,1.0f);
}