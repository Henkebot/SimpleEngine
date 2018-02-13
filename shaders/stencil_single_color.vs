#version 440
layout(location = 0) in vec3 pos;

layout(location = 2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uniformNormals;

void main()
{
	vec3 normal = normalize(normals);
	vec3 finalPos = (uniformNormals > 0) ? (pos + (normal * 0.1)) : pos;
	gl_Position = projection * view * model * vec4(finalPos,1.0);
}