#version 420

//
// Uniforms
//
uniform mat4 mMatrix;
uniform mat4 pMatrix;
uniform mat4 mvMatrix;
uniform mat4 mvpMatrix;
uniform mat3 nMatrix;

uniform vec2 Viewport;
uniform sampler2D TexTerrainHeight;

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



float distTess(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	float g_tessellatedTriWidth = 20.0;

	vec4 samp = texture(TexTerrainHeight, t0);
	p0.y += samp[0] * 20;
	samp = texture(TexTerrainHeight, t1);
	p1.y += samp[0] * 20;

	vec4 center = 0.5 * (p0 + p1);
	float radius = distance(p0, p1) / 2;

	vec4 sc0 = mvMatrix * center;
	vec4 sc1 = sc0;
	sc0.x -= radius;
	sc1.x += radius;

	vec4 clip0 = pMatrix * sc0;
	vec4 clip1 = pMatrix * sc1;

	clip0 /= clip0.w;
	clip1 /= clip1.w;

	clip0.xy *= Viewport;
	clip1.xy *= Viewport;

	float d = distance(clip0, clip1);

	// g_tessellatedTriWidth is desired pixels per tri edge
	return clamp(d / g_tessellatedTriWidth, 0, 64);
}

void main(void)
{
	// Outer tessellation level
	gl_TessLevelOuter[0] = distTess(gl_in[3].gl_Position, gl_in[0].gl_Position, tcs_terrainTexCoord[3], tcs_terrainTexCoord[0]);
	gl_TessLevelOuter[1] = distTess(gl_in[0].gl_Position, gl_in[1].gl_Position, tcs_terrainTexCoord[0], tcs_terrainTexCoord[1]);
	gl_TessLevelOuter[2] = distTess(gl_in[1].gl_Position, gl_in[2].gl_Position, tcs_terrainTexCoord[1], tcs_terrainTexCoord[2]);
	gl_TessLevelOuter[3] = distTess(gl_in[2].gl_Position, gl_in[3].gl_Position, tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);

	//gl_TessLevelOuter[0] = 1.0;
	//gl_TessLevelOuter[1] = 1.0;
	//gl_TessLevelOuter[2] = 1.0;
	//gl_TessLevelOuter[3] = 1.0;

	// Inner tessellation level
	gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[3]);
	gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[2] + gl_TessLevelOuter[1]);

	// Pass the patch verts along
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// Pass texture coordinates along
	tcs_terrainTexCoord[gl_InvocationID] = vs_terrainTexCoord[gl_InvocationID];
	tcs_patchTexCoord[gl_InvocationID] = vs_patchTexCoord[gl_InvocationID];
}