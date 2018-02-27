#version 440

//
// Uniforms
//
uniform	mat4 World;
uniform mat4 View;

uniform sampler2D TexTerrainHeight;
uniform float TerrainHeightOffset;
uniform float TerrainLength;
uniform float TerrainWidth;

uniform vec3 TerrainOrigin;


layout(quads, fractional_even_spacing) in;

patch in float gl_TessLevelOuter[4];
patch in float gl_TessLevelInner[2];

in vec2 tcs_terrainTexCoord[];
in vec2 tcs_patchTexCoord[];
in float tcs_tessLevel[];

//
// Outputs
//
out vec2 tes_terrainTexCoord;
out vec2 tes_patchTexCoord;
out float tes_tessLevel;
out vec3 tes_modelFrag;

vec2 calcTerrainTexCoord(in vec4 pos)
{
	return vec2((pos.x - TerrainOrigin.x) / TerrainWidth, (pos.z - TerrainOrigin.z) / TerrainWidth);
}

vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
	vec4 a = mix(v0, v1, gl_TessCoord.x);
	vec4 b = mix(v3, v2, gl_TessCoord.x);
	return mix(a, b, gl_TessCoord.y);
}

vec2 interpolateVec2(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)
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
	vec2 terrainTexCoord = interpolateVec2(tcs_terrainTexCoord[0], tcs_terrainTexCoord[1], tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);

	// Increase UV count if patch is bigger
	float sideLength = length(gl_in[0].gl_Position - gl_in[1].gl_Position) / 16.0f;

	tes_patchTexCoord = interpolateVec2(tcs_patchTexCoord[0], tcs_patchTexCoord[1], tcs_patchTexCoord[2], tcs_patchTexCoord[3]) * sideLength;

	// Sample the heightmap and offset y position of vertex
	vec4 samp = texture(TexTerrainHeight, terrainTexCoord);
	gl_Position.y = samp[0] * TerrainHeightOffset;

	tes_terrainTexCoord = terrainTexCoord;
	tes_tessLevel = tcs_tessLevel[0];
	tes_modelFrag = vec3(World * gl_Position);
	gl_Position = View * World * gl_Position;
}