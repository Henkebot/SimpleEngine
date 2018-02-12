#version 440
layout(location = 0) in vec3 in_vert;

uniform vec3 CameraRightWorld;
uniform vec3 CameraUpWorld;
uniform mat4 ViewProj;
uniform vec3 BillboardPos;
uniform vec2 BillboardSize;

void main()
{
	vec3 particleCenter_worldSpace = BillboardPos;
	vec3 vertexPosition_worldSpace = particleCenter_worldSpace + CameraRightWorld * in_vert.y + CameraUpWorld * in_vert.x;
	gl_Position = ViewProj * vec4(vertexPosition_worldSpace, 1.0f);
}
