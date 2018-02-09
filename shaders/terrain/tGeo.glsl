#version 440

//
// Uniforms
//
uniform vec2 Viewport;
uniform float ToggleWireframe;

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
in vec4 tes_splatMap[];

//
// Outputs
//
layout(triangle_strip, max_vertices = 4) out;

out vec4 gs_wireColor;
noperspective out vec4 gs_splatMap;
noperspective out vec3 gs_edgeDist;
out vec2 gs_terrainTexCoord;
out vec2 gs_patchTexCoord;

vec4 wireframeColor()
{
	if (tes_tessLevel[0] == 64.0)
		return vec4(0.0, 0.0, 1.0, 1.0);
	else if (tes_tessLevel[0] >= 32.0)
		return vec4(0.0, 1.0, 1.0, 1.0);
	else if (tes_tessLevel[0] >= 16.0)
		return vec4(1.0, 1.0, 0.0, 1.0);
	else if (tes_tessLevel[0] >= 8.0)
		return vec4(1.0, 1.0, 1.0, 1.0);
	else
		return vec4(1.0, 0.0, 0.0, 1.0);
}

void main(void)
{
	vec4 wireColor = wireframeColor();

	// Calculate edge distances for wireframe
	float ha, hb, hc;
	if (ToggleWireframe == 1.0)
	{
		vec2 p0 = vec2(Viewport * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w));
		vec2 p1 = vec2(Viewport * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w));
		vec2 p2 = vec2(Viewport * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w));

		float a = length(p1 - p2);
		float b = length(p2 - p0);
		float c = length(p1 - p0);
		float alpha = acos((b*b + c * c - a * a) / (2.0*b*c));
		float beta = acos((a*a + c * c - b * b) / (2.0*a*c));
		ha = abs(c * sin(beta));
		hb = abs(c * sin(alpha));
		hc = abs(b * sin(alpha));
	}
	else
	{
		ha = hb = hc = 0.0;
	}

	vec4 pos[3] = vec4[](gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	vec3 normal = vec3(cross(pos[1].xyz - pos[0].xyz, pos[2].xyz - pos[0].xyz));

	if (dot(-pos[0].xyz, normal) < 0)
	{

		// Output verts
		for (int i = 0; i < gl_in.length(); ++i)
		{
			gl_Position = pMatrix * gl_in[i].gl_Position;
			gs_splatMap = tes_splatMap[i];
			gs_terrainTexCoord = tes_terrainTexCoord[i];
			gs_patchTexCoord = tes_patchTexCoord[i];
			gs_wireColor = wireColor;

			if (i == 0)
				gs_edgeDist = vec3(ha, 0, 0);
			else if (i == 1)
				gs_edgeDist = vec3(0, hb, 0);
			else
				gs_edgeDist = vec3(0, 0, hc);

			EmitVertex();
		}

		// This closes the the triangle
		gl_Position = pMatrix * gl_in[0].gl_Position;
		gs_splatMap = tes_splatMap[0];
		gs_edgeDist = vec3(ha, 0, 0);
		gs_terrainTexCoord = tes_terrainTexCoord[0];
		gs_patchTexCoord = tes_patchTexCoord[0];
		gs_wireColor = wireColor;
		EmitVertex();

		EndPrimitive();

	}
}