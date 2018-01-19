#version 440
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

out vec4 fColor;

void main()
{
	fColor = vColor;
	gl_Position = vPosition;
}