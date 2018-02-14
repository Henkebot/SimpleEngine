#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 World;
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.FragPos = vec3(World * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(World))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = Projection * View * World * vec4(aPos, 1.0);
}