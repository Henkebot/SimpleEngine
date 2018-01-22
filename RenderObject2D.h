#ifndef ENTITY_2D
#define ENTITY_2D


#include <GL\glew.h>
#include "Shader.h"
#include <vector>
#include "RenderObject.h"



struct Properties2D
{
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 size;
	glm::vec3 color;
};

class RenderObject2D : public RenderObject
{
private:
	glm::vec2 m_Position;
	glm::vec2 m_Rotation;
	glm::vec2 m_Size;
	glm::vec3 m_Color;
public:
	RenderObject2D(const glm::vec2& position = glm::vec2(0.0f, 0.0f), 
					const glm::vec2& size = glm::vec2(1.0f, 1.0f), 
					const glm::vec2& rotation = glm::vec2(1.0f, 1.0f), 
					const glm::vec4 color = glm::vec4(1.0f, 0.5f,0.5f,1.0f));

	//RenderObject2D(ShaderInfo* shaders, VertexData vert, Properties2D prop);
	
	void updateUniforms() override;
	
};
#endif