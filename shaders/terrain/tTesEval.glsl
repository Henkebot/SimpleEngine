#version 420

//
// Uniforms
//
uniform	mat4 mMatrix;
uniform	mat4 pMatrix;
uniform	mat4 mvMatrix;
uniform	mat4 mvpMatrix;
uniform	mat3 nMatrix;


uniform sampler2D TexTerrainHeight;
uniform sampler2D TexTerrainHeight2;

uniform float TerrainHeightOffset;

//
// Inputs
//
layout(quads, fractional_even_spacing) in;

patch in float gl_TessLevelOuter[4];
patch in float gl_TessLevelInner[2];

in vec2 tcs_terrainTexCoord[];
in float tcs_tessLevel[];

//
// Outputs
//
out vec2 tes_terrainTexCoord;
out float tes_tessLevel;



vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec2 interpolate2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
{
	vec2 a = mix(v0, v1, gl_TessCoord.x);
	vec2 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

void main()
{
	// Calculate the vertex position using the four original points and interpolate depneding on the tessellation coordinates.	
	gl_Position = interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);

	// Terrain heightmap coords
	vec2 terrainTexCoord = interpolate2(tcs_terrainTexCoord[0], tcs_terrainTexCoord[1], tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);

	// Sample the heightmap and offset y position of vertex
	vec4 samp = texture(TexTerrainHeight, terrainTexCoord);
	vec4 samp2 = texture(TexTerrainHeight2, terrainTexCoord);
	gl_Position.y = samp[0] * TerrainHeightOffset;

	// Project the vertex to clip space and send it along
	gl_Position = pMatrix * mvMatrix * gl_Position;

	tes_terrainTexCoord = terrainTexCoord;
	tes_tessLevel = tcs_tessLevel[0];
}