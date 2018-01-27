#version 440
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;

out vec4 vsColor;

void main()
{
	vsColor = vColor;
	gl_Position = Projection * View * World * vPosition;
}
