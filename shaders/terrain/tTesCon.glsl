#version 440

//
// Uniforms
//
uniform	mat4 World;
uniform	mat4 View;

uniform sampler2D TexTerrainHeight;
uniform float TerrainHeightOffset;

uniform float tscale_negx;
uniform float tscale_negz;
uniform float tscale_posx;
uniform float tscale_posz;

//
// Inputs
//
in vec2 vs_terrainTexCoord[];
in vec2 vs_patchTexCoord[];

//
// Outputs
//
layout(vertices = 4) out;

patch out float gl_TessLevelOuter[4];
patch out float gl_TessLevelInner[2];

out vec2 tcs_terrainTexCoord[];
out vec2 tcs_patchTexCoord[];
out float tcs_tessLevel[];

/**
* Dynamic level of detail using camera distance algorithm.
*/
float dlodCameraDistance(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	vec4 samp = texture(TexTerrainHeight, t0);
	p0.y = samp[0] * TerrainHeightOffset;
	samp = texture(TexTerrainHeight, t1);
	p1.y = samp[0] * TerrainHeightOffset;

	vec4 view0 = View * World * p0;
	vec4 view1 = View * World * p1;

	float MinDepth = 100.0;
	float MaxDepth = 1024.0;

	float d0 = clamp((abs(view0.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
	float d1 = clamp((abs(view1.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);

	float t = mix(64, 2, (d0 + d1) * 0.5);
	
	if (t <= 2.0)
	{
		return 2.0;
	}
	if (t <= 4.0)
	{
		return 4.0;
	}
	if (t <= 8.0)
	{
		return 8.0;
	}
	if (t <= 16.0)
	{
		return 16.0;
	}
	if (t <= 32.0)
	{
		return 32.0;
	}

	return 64.0;
}


void main(void)
{
	// Outer tessellation level
	gl_TessLevelOuter[0] = dlodCameraDistance(gl_in[3].gl_Position, gl_in[0].gl_Position, tcs_terrainTexCoord[3], tcs_terrainTexCoord[0]);
	gl_TessLevelOuter[1] = dlodCameraDistance(gl_in[0].gl_Position, gl_in[1].gl_Position, tcs_terrainTexCoord[0], tcs_terrainTexCoord[1]);
	gl_TessLevelOuter[2] = dlodCameraDistance(gl_in[1].gl_Position, gl_in[2].gl_Position, tcs_terrainTexCoord[1], tcs_terrainTexCoord[2]);
	gl_TessLevelOuter[3] = dlodCameraDistance(gl_in[2].gl_Position, gl_in[3].gl_Position, tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);

	// To match the patches with eachother, basically just lower the tesselation level to a smaller
	if (tscale_negx >= 2.0)
	{
		gl_TessLevelOuter[0] = min(64.0, gl_TessLevelOuter[0] * tscale_negx);
		
	}
	if (tscale_negz >= 2.0)
	{
		gl_TessLevelOuter[1] = min(64.0, gl_TessLevelOuter[1] * tscale_negz);
		
	}
	if (tscale_posx >= 2.0)
	{
		gl_TessLevelOuter[2] = min(64.0, gl_TessLevelOuter[2] * tscale_posx);
	
	}
	if (tscale_posz >= 2.0)
	{
		gl_TessLevelOuter[3] = min(64.0, gl_TessLevelOuter[3] * tscale_posz);
	
	}

	// Inner tessellation level is simply an average of the outer
	gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[3]);
	gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[2] + gl_TessLevelOuter[1]);

	// Pass the patch verts along
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// Output heightmap coordinates
	tcs_terrainTexCoord[gl_InvocationID] = vs_terrainTexCoord[gl_InvocationID];
	tcs_patchTexCoord[gl_InvocationID] = vs_patchTexCoord[gl_InvocationID];
	
	
	// Output tessellation level (used for wireframe coloring)
	tcs_tessLevel[gl_InvocationID] = gl_TessLevelOuter[0];
}