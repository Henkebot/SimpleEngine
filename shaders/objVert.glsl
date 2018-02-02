#version 440
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normals;
layout(location = 3) in vec3 in_tangents;
layout(location = 4) in vec3 in_bitangents;


uniform mat4 Projection, View, World;
uniform vec3 Light_pos;

out DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 normals;
	vec3 lightDir;
	vec3 cameraDir;

	vec3 lightDir_tangentSpace;
	vec3 cameraDir_tangentSpace;
} vs_out;


void main()
{
	vs_out.uvs = in_uvs;
	vs_out.normals = in_normals;
	vs_out.pos = (World * vec4(in_pos,1.0f)).xyz;
	vec3 vertexPosition_cameraSpace = (View * vec4(vs_out.pos,1)).xyz;
	vs_out.cameraDir = vec3(0,0,0) - vertexPosition_cameraSpace;

	vec3 LightPosition_cameraSpace = (View * vec4(Light_pos,1)).xyz;
	vs_out.lightDir = LightPosition_cameraSpace + vs_out.cameraDir;

	mat3 MV3x3 = mat3(View*World);

	vec3 vertexTangent_cameraSpace = MV3x3 * in_tangents;
	vec3 vertexBitangent_cameraSpace = MV3x3 * in_bitangents;
	vec3 vertexNormal_cameraSpace = MV3x3 * in_normals;

	mat3 TBN = transpose(mat3(
		vertexTangent_cameraSpace,
		vertexBitangent_cameraSpace,
		vertexNormal_cameraSpace
	));


	vs_out.lightDir_tangentSpace = TBN * vs_out.lightDir;
	vs_out.cameraDir_tangentSpace = TBN * vs_out.cameraDir;

	gl_Position = Projection * View * vec4(vs_out.pos,1.0f);
}