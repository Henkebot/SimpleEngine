#version 440
layout(location = 0) in vec4 in_vertex;

uniform mat4 Projection, View, World;

out vec3 vs_TextureCoord;

void main()
{
	gl_Position = Projection * View * World * in_vertex;
	vs_TextureCoord = normalize(in_vertex.xyz) * -1;
}