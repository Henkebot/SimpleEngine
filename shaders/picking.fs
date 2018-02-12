#version 440
uniform uint drawIndex;
uniform uint objectIndex;

out vec3 FragColor;

void main()
{
	FragColor = vec3(float(objectIndex), float(drawIndex), float(gl_PrimitiveID + 1));
	//FragColor = vec3(1,1,1);
}