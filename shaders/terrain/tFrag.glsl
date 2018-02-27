#version 440

uniform sampler2D TexTerrainHeight;
uniform sampler2D NormalMap;
uniform sampler2D BlendMap;
uniform sampler2D TexBase;
uniform sampler2D TexBaseNormal;
uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

uniform vec3 Light_Pos;
uniform vec3 camera_pos;

//
// Inputs
//
in vec2 gs_terrainTexCoord;
in vec2 gs_patchTexCoord;
in vec3 gs_normal;
in vec3 gs_fragPos;

//
// Ouputs
//
layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	vec3 LightColor = vec3(1, 1, 1);
	float lightPower = 90000;

	vec4 blendMapColour = texture(BlendMap, gs_terrainTexCoord);

	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);

	vec3 color4 = texture(Tex2, gs_patchTexCoord).rgb * backTextureAmount;
	vec3 color3 = color4 + texture(TexBase, gs_patchTexCoord).rgb * blendMapColour.b;
	vec3 color2 = color3 + texture(Tex1, gs_patchTexCoord).rgb * blendMapColour.g;
	vec3 color = color2 + texture(Tex0, gs_patchTexCoord).rgb * blendMapColour.r;
	
	// Ambient
	vec3 ambient = 0.3 * color;

	// Diffuse
	vec3 lightDir = normalize(Light_Pos - gs_fragPos);
	float diff = clamp(dot(lightDir, gs_normal), 0.0, 1.0);
	vec3 diffuse = diff * color;

	float distanceSquare = length(gs_fragPos - Light_Pos) * length(gs_fragPos - Light_Pos);

	vec3 lightning = ((ambient + diffuse) *  LightColor * lightPower) / distanceSquare;

	fragColor = vec4(lightning,1);
}