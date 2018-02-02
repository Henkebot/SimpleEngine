#version 440
layout(location = 0) out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D ambientTexture;
uniform sampler2D specularTexture;
uniform sampler2D bumpMap;

uniform vec3 Light_pos;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;

in DATA
{
	vec3 pos;
	vec2 uvs;
	vec3 normals;
	vec3 lightDir;
	vec3 cameraDir;

	vec3 lightDir_tangentSpace;
	vec3 cameraDir_tangentSpace;
} fs_in;

void main()
{

	vec3 LightColor = vec3(1,1,1);
	float lightPower =200.0f;

	vec3 MaterialDiffuseColor = texture(diffuseTexture, fs_in.uvs).rgb;
	vec3 MaterialAmbientColor = texture(ambientTexture, fs_in.uvs).rgb * 0.1;
	vec3 MaterialSpecularColor = texture(specularTexture, fs_in.uvs).rgb;

	vec3 TextureNormal_tangentSpace = normalize(texture( bumpMap, vec2(fs_in.uvs.x,-fs_in.uvs.y) ).rgb *2.0 - 1.0);
	
	float distance = length(Light_pos - fs_in.pos);

	vec3 n = TextureNormal_tangentSpace;
	vec3 l = normalize(fs_in.lightDir_tangentSpace);
	float cosTheta = clamp(dot(n,l), 0, 1);

	vec3 E = normalize(fs_in.cameraDir_tangentSpace);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp(dot(E,R), 0, 1);

	vec3 finalColor = MaterialAmbientColor +
					(MaterialDiffuseColor * LightColor* lightPower * cosTheta / (distance*distance) )+
					(MaterialSpecularColor * LightColor* lightPower * pow(cosAlpha,96) / (distance*distance));
	outColor = vec4(finalColor,1.0);
}
