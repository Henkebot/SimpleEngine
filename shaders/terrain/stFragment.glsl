#version 420

//
// Uniforms
//
uniform mat4 mMatrix;
uniform mat4 pMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 nMatrix;

uniform sampler2D TexBase;
uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Tex3;

uniform float TessLevelInner;
uniform float TessLevelOuter;
uniform float ToggleWireframe;

//
// Inputs
//
in vec4 gs_wireColor;
in vec2 gs_patchTexCoord;
in vec3 gs_lightIntensity;
noperspective in vec4 gs_splatMap;
noperspective in vec3 gs_edgeDist;

//
// Ouputs
//
layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	vec3 tex0, tex1, tex2, tex3;
	vec3 color = texture(TexBase, gs_patchTexCoord).rgb;

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




	// Wireframe stuff
	float d = min(gs_edgeDist.x, gs_edgeDist.y);
	d = min(d, gs_edgeDist.z);

	float LineWidth = 0.75;
	float mixVal = smoothstep(LineWidth - 1, LineWidth + 1, d);

	if (ToggleWireframe == 1.0)
		fragColor = mix(gs_wireColor, vec4(color, 1.0), mixVal);
	else
		fragColor = vec4(0.5, 0.5, 0.5, 1.0) * vec4(color, 1.0) + vec4(gs_lightIntensity, 1.0) * vec4(color, 1.0);
	//fragColor = vec4(gs_lightIntensity, 1.0) * vec4(color, 1.0);

}