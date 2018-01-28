#version 440
layout(location = 0) out vec4 outColor;

in vec2 out_uvs;
in vec3 out_normals;

void main()
{
	outColor = vec4(out_normals,1.0f);
}
