#version 440
layout(location = 0) in vec3 pos;

layout(location = 2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos + (normals *0.02),1.0);
}