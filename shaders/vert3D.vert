#version 440
#pragma optimize(off)
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 Projection;
uniform mat4 World;
uniform mat4 View;
uniform vec2 light_pos;

out DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
} vs_out;

void main()
{
	vs_out.color = vColor;
	vs_out.position = World * vPosition;
	vs_out.lightPos = vec4(light_pos, -2.0f ,1.0f);
	gl_Position = Projection * View * vs_out.position;
}