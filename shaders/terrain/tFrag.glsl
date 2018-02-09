#version 440

//
// Uniforms
//
uniform	mat4 mMatrix;
uniform	mat4 pMatrix;
uniform	mat4 mvMatrix;
uniform	mat4 mvpMatrix;
uniform	mat3 nMatrix;

uniform float tileScale;
uniform float ToggleWireframe;
uniform sampler2D TexTerrainHeight;
uniform sampler2D TexBase;
uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

//
// Inputs
//
in vec4 gs_wireColor;
noperspective in vec4 gs_splatMap;
noperspective in vec3 gs_edgeDist;
in vec2 gs_terrainTexCoord;
in vec2 gs_patchTexCoord;

//
// Ouputs
//
layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	//vec4 color = vec4(mix(0.0, 1.0, tileScale / 1000.0), mix(1.0, 0.0, tileScale / 1000.0), 0.0, 1.0);
	vec3 color = texture(TexBase, gs_patchTexCoord).rgb;
	
	vec3 tex0, tex1, tex2, tex3;
	if (gs_splatMap[0] > 0)
	{
		tex0 = texture(Tex0, gs_patchTexCoord).rgb;
		color = tex0 * gs_splatMap[0] + color * (1.0 - gs_splatMap[0]);
	}
	if (gs_splatMap[1] > 0)
	{
		tex1 = texture(Tex1, gs_patchTexCoord).rgb;
		color = tex1 * gs_splatMap[1] + color * (1.0 - gs_splatMap[1]);
	}
	if (gs_splatMap[2] > 0)
	{
		tex2 = texture(Tex2, gs_patchTexCoord).rgb;
		color = tex2 * gs_splatMap[2] + color * (1.0 - gs_splatMap[2]);
	}
	if (gs_splatMap[3] > 0)
	{
		tex3 = texture(Tex3, gs_patchTexCoord).rgb;
		color = tex3 * gs_splatMap[3] + color * (1.0 - gs_splatMap[3]);
	}

	// Wireframe junk
	float d = min(gs_edgeDist.x, gs_edgeDist.y);
	d = min(d, gs_edgeDist.z);

	float LineWidth = 0.75;
	float mixVal = smoothstep(LineWidth - 1, LineWidth + 1, d);

	
		fragColor = vec4(color,1);
}