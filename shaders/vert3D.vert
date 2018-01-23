#version 440
#pragma optimize(off)
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 World;
uniform vec3 light_pos;

out DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
} vertex;

void main()
{
	vertex.color = vColor;
	vertex.position = World * vPosition;
	vertex.lightPos = World * vec4(light_pos,1.0f);

	gl_Position = vertex.position;
}