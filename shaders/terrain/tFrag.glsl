#version 440

//
// Uniforms
//
uniform	mat4 mMatrix;
uniform	mat4 pMatrix;
uniform	mat4 mvMatrix;
uniform	mat4 mvpMatrix;
uniform	mat3 nMatrix;

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
uniform vec3 Camera_Pos;

//
// Inputs
//
in vec2 gs_terrainTexCoord;
in vec2 gs_patchTexCoord;
in vec3 gs_tangent;
in vec3 gs_bitangent;
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
	vec3 normalMap = normalize(texture(NormalMap, gs_terrainTexCoord).rgb * 2.0 -1.0);
	mat3 normalMatrix = transpose(inverse(mat3(mMatrix)));
	vec3 T = normalize(normalMatrix * gs_tangent);
	vec3 N = normalize(normalMatrix * gs_normal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	vec3 TangentLightPos = TBN * Light_Pos;
	vec3 TangentViewPos = TBN * Camera_Pos;
	vec3 TangentFragPos = TBN * gs_fragPos;

	vec4 blendMapColour = texture(BlendMap, gs_terrainTexCoord);
	float backTextureAmount = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);
	vec3 color4 = texture(Tex2, gs_patchTexCoord).rgb * backTextureAmount;
	vec3 color3 = color4 + texture(TexBase, gs_patchTexCoord).rgb * blendMapColour.b;
	vec3 color2 = color3 + texture(Tex1, gs_patchTexCoord).rgb * blendMapColour.g;
	vec3 color = color2 + texture(Tex0, gs_patchTexCoord).rgb * blendMapColour.r;
	vec3 bumpNormal = normalize(texture(NormalMap, gs_terrainTexCoord).rgb * 2.0 - 1.0);
	
	// Ambient
	vec3 ambient = 0.3 * color;

	// Diffuse
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diff = clamp(dot(lightDir, bumpNormal), 0.0, 1.0);
	vec3 diffuse = diff * color;

	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(bumpNormal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * LightColor;

	float distanceSquare = length(TangentFragPos - TangentLightPos) * length(TangentFragPos - TangentLightPos);

	vec3 lightning = ((ambient + diffuse) *  LightColor * lightPower) / distanceSquare;


	
	fragColor = vec4(lightning,1);
}