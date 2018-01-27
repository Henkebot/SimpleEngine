#version 440

out vec4 fsColor;
in vec4 vsColor;

void main()
{
	fsColor = vsColor;
}