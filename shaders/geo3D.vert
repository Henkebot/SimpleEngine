#version 440
#pragma optimize(off)

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 Projection;
uniform mat4 View;

in DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;

} vertices[];

out DATA
{
	vec4 position;
	vec4 color;
	vec4 lightPos;
	vec4 normal;
} geo_out;

void main()
{
	int i;
	for(i = 0; i < gl_in.length();i++)
	{
		geo_out.position = vertices[i].position;
		geo_out.color = vertices[i].color;
		geo_out.lightPos = vertices[i].lightPos;
		
		// Calculate normal
		vec4 pos[3] = vec4[](gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
		vec3 normal = vec3( cross( pos[1].xyz-pos[0].xyz , pos[2].xyz-pos[0].xyz ));

		geo_out.normal = vec4(normalize(normal),1.0f);

		gl_Position = Projection * View *vertices[i].position;

		EmitVertex();
	}

	EndPrimitive();
	
}
