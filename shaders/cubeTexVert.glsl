#version 440
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTex;

uniform mat4 Projection, View, World;

out vec2 texCoord;

void main()
{
	texCoord = inTex;

	gl_Position = Projection * View * World * vec4(inPos,1.0f);
}