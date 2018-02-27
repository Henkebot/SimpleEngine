#version 440

uniform	mat4 World;
uniform	mat4 Projection;
uniform mat4 View;

uniform vec3 camera_pos;

//
// Inputs
//
layout(triangles) in;
in vec2 tes_terrainTexCoord[];
in vec2 tes_patchTexCoord[];
in float tes_tessLevel[];
in vec3 tes_modelFrag[];

//
// Outputs
//
layout(triangle_strip, max_vertices = 4) out;

out vec2 gs_terrainTexCoord;
out vec2 gs_patchTexCoord;
out vec3 gs_normal;
out vec3 gs_fragPos;


void main(void)
{

	vec4 pos[3] = vec4[](gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	vec3 normal = vec3(cross(pos[1].xyz - pos[0].xyz, pos[2].xyz - pos[0].xyz));

	vec3 pos2[3] = vec3[](tes_modelFrag[0], tes_modelFrag[1], tes_modelFrag[2]);
	vec3 normal2 = normalize(vec3(cross(pos2[2].xyz - pos2[0].xyz, pos2[1].xyz - pos2[0].xyz)));
	
	// Culling
	if (dot(-pos[0].xyz, normal) < 0)
	{

		float tileSize = 16.0f;
		float tileLength = 1.0f;
		
		// Output verts
		for (int i = 0; i < gl_in.length(); i++)
		{
			gl_Position = Projection * gl_in[i].gl_Position;

			gs_fragPos = tes_modelFrag[i];
			gs_terrainTexCoord = tes_terrainTexCoord[i];
			gs_patchTexCoord = tes_patchTexCoord[i] * tileLength;

			gs_normal = normal2;
			
			EmitVertex();
		}

		// This closes the the triangle
		gl_Position = Projection * gl_in[0].gl_Position;
		gs_terrainTexCoord = tes_terrainTexCoord[0];
		gs_patchTexCoord = tes_patchTexCoord[0] * tileLength;
		gs_fragPos = tes_modelFrag[0];

		gs_normal = normal2;

		EmitVertex();

		EndPrimitive();

	}
}