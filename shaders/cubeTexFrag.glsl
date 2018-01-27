#version 440
layout(location = 0) out vec4 outColor;

uniform sampler2D texture1;

in vec2 texCoord;

void main()
{
	vec4 color = texture(texture1, texCoord);
	outColor = color;
}