#version 440

//
// Uniforms
//
uniform	mat4 mMatrix;
uniform	mat4 pMatrix;
uniform	mat4 mvMatrix;
uniform	mat4 mvpMatrix;
uniform	mat3 nMatrix;
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
out vec3 gs_tangent;
out vec3 gs_bitangent;
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
		// Compute Bitangents and normals

		vec3 v0 = tes_modelFrag[0];
		vec3 v1 = tes_modelFrag[1];
		vec3 v2 = tes_modelFrag[2];

		vec2 uv0 = tes_patchTexCoord[0];
		vec2 uv1 = tes_patchTexCoord[1];
		vec2 uv2 = tes_patchTexCoord[2];

		vec3 deltaPos1 = v1 - v0;
		vec3 deltaPos2 = v2 - v0;

		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		vec3 tangent;
		tangent.x = r * (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x);
		tangent.y = r * (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y);
		tangent.z = r * (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z);
		tangent = normalize(tangent);

		vec3 bitangent;
		bitangent.x = r * (-deltaUV2.x * deltaPos1.x + deltaUV1.x * deltaPos2.x);
		bitangent.y = r * (-deltaUV2.x * deltaPos1.y + deltaUV1.x * deltaPos2.y);
		bitangent.z = r * (-deltaUV2.x * deltaPos1.z + deltaUV1.x * deltaPos2.z);
		bitangent = normalize(bitangent);



		// Output verts
		for (int i = 0; i < gl_in.length(); ++i)
		{
			gl_Position = pMatrix * gl_in[i].gl_Position;
			gs_fragPos = tes_modelFrag[i];
			gs_terrainTexCoord = tes_terrainTexCoord[i];
			gs_patchTexCoord = tes_patchTexCoord[i];
			gs_bitangent = bitangent;
			gs_tangent = tangent;
			gs_normal = normal2;
			
			EmitVertex();
		}

		// This closes the the triangle
		gl_Position = pMatrix * gl_in[0].gl_Position;
		gs_terrainTexCoord = tes_terrainTexCoord[0];
		gs_patchTexCoord = tes_patchTexCoord[0];
		gs_fragPos = tes_modelFrag[0];
		gs_bitangent = bitangent;
		gs_tangent = tangent;
		gs_normal = normal2;

		EmitVertex();

		EndPrimitive();

	}
}