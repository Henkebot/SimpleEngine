#version 420

//
// Uniforms
//
uniform mat4 mMatrix;
uniform mat4 pMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 nMatrix;

uniform float TerrainWidth, TerrainHeight;
uniform vec2 TerrainOrigin;

//
// Inputs
//
in vec4 a_vertex;
in vec2 a_patchTexCoord;

// Outputs
//
out vec2 vs_terrainTexCoord;
out vec2 vs_patchTexCoord;


vec2 calcTerrainTexCoord(in vec4 pos)
{
	return vec2(abs(pos.x - TerrainOrigin.x) / TerrainWidth, abs(pos.z - TerrainOrigin.y) / TerrainHeight);
}

void main(void)
{
	// Calcuate texture coordantes (u,v) relative to entire terrain
	vec4 p = mMatrix * a_vertex;
	vs_terrainTexCoord = calcTerrainTexCoord(p);

	// Send vertex position along
	gl_Position = a_vertex;

	// Local patch texture coordinates
	vs_patchTexCoord = a_patchTexCoord;
}