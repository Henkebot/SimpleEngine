#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D bumpMap;
uniform samplerCube depthMap;

uniform vec3 Light_Pos;
uniform vec3 Camera_Pos;

uniform float far_plane;


// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = fs_in.FragPos - Light_Pos;
	// use the fragment to light vector to sample from the depth map    
	// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	// closestDepth *= far_plane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
  
	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(Camera_Pos - fs_in.FragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
		closestDepth *= far_plane;   // undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);
       
	// display closestDepth as debug (to visualize depth cubemap)
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
       
	return shadow;
}

void main()
{           
	vec3 LightColor = vec3(1,0,0);
	float lightPower = 40;

    vec3 MaterialDiffuseColor = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 BumpNormal = normalize(texture(bumpMap, fs_in.TexCoords).rgb * 2.0 -1.0);
	
    // ambient
    vec3 ambient = 0.3 * MaterialDiffuseColor;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = clamp(dot(lightDir, BumpNormal), 0.0, 1.0);
    vec3 diffuse = diff * MaterialDiffuseColor;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(BumpNormal, halfwayDir), 0.0), 16.0);
    vec3 specular = spec * LightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPos);   
	
	float distanceSquare = length(fs_in.TangentFragPos - fs_in.TangentLightPos) * length(fs_in.TangentFragPos - fs_in.TangentLightPos);

    vec3 lighting = (ambient + (1.0 - shadow) * 
	((diffuse * LightColor * lightPower)/distanceSquare) + 
	((specular* LightColor * lightPower)/distanceSquare)
	);    
    
    FragColor = vec4(lighting, 1.0);
}