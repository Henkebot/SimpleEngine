#version 440

//
// Uniforms
//
uniform	mat4 World;
uniform	mat4 Projection;
uniform	mat4 View;

uniform float tileScale;
uniform float TerrainLength;
uniform float TerrainWidth;

uniform vec3 TerrainOrigin;

//
// Inputs
//
in vec4 a_vertex;
in vec2 uvs;
//
// Outputs
//
out vec2 vs_terrainTexCoord;
out vec2 vs_patchTexCoord;

vec2 calcTerrainTexCoord(in vec4 pos)
{
	return vec2((pos.x - TerrainOrigin.x) / TerrainWidth, (pos.z - TerrainOrigin.z) / TerrainWidth);
}

void main(void)
{
	vec4 p = World * vec4(a_vertex.xyz * tileScale, 1.0);
	vs_terrainTexCoord = calcTerrainTexCoord(p);
	vs_patchTexCoord = uvs;

	// Send vertex position along
	gl_Position = vec4(a_vertex.xyz * tileScale, 1.0);
}