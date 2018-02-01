#version 440
layout(location = 0) out vec4 fragColor;

in vec3 vs_TextureCoord;

uniform samplerCube SkyboxTexture;

void main()
{
	fragColor = texture(SkyboxTexture, vs_TextureCoord);
}
