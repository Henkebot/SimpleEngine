#version 440
layout (location = 0) in vec3 vs_Position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	gl_Position = P * V * M * vec4(vs_Position, 1.0);
}