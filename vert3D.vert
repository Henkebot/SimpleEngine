#version 440

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 Projection;
uniform mat4 World;
uniform vec2 light_pos;

out DATA
{
	vec4 pos;
	vec4 color;
	vec4 lightPos;
} vs_out;

void main()
{
	vs_out.color = vColor;
	vs_out.pos = World * vPosition;
	vs_out.lightPos = vec4(light_pos, 0.0f ,1.0f);

	gl_Position = Projection * vs_out.pos;
}