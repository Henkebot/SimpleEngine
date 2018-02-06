#version 420

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

//
// Inputs
//
in vec4 gs_wireColor;
noperspective in vec3 gs_edgeDist;
in vec2 gs_terrainTexCoord;

//
// Ouputs
//
layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	//vec4 color = vec4(mix(0.0, 1.0, tileScale / 1000.0), mix(1.0, 0.0, tileScale / 1000.0), 0.0, 1.0);
	vec4 color = texture(TexTerrainHeight, gs_terrainTexCoord);

	// Wireframe junk
	float d = min(gs_edgeDist.x, gs_edgeDist.y);
	d = min(d, gs_edgeDist.z);

	float LineWidth = 0.75;
	float mixVal = smoothstep(LineWidth - 1, LineWidth + 1, d);

	if (ToggleWireframe == 1.0)
		fragColor = mix(gs_wireColor, color, mixVal);
	else
		fragColor = color;
}