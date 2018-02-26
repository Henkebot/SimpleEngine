#version 440
out vec4 FragColor;
in  vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

uniform vec3 Light_pos;
uniform vec3 Camera_pos;

void main()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Albedo = texture(gColorSpec, TexCoords).rgb;
	float Specular = texture(gColorSpec, TexCoords).a;

	vec3 lighting = Albedo * 0.1;
	vec3 ViewDir = normalize(Camera_pos - FragPos);
	vec3 lightDir = normalize(Light_pos - FragPos);
	vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo;

	vec3 halfwayDir = normalize(lightDir + ViewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
	vec3 specular = vec3(1,1,1)*spec * Specular;

	lighting += diffuse + specular;

	FragColor = vec4(lighting,1.0);

}