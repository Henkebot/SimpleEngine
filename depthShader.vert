#version 440 
layout (location = 0) in vec3 pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 World;

void main()
{
	gl_Position = lightSpaceMatrix * World * vec4(pos, 1.0);
}
