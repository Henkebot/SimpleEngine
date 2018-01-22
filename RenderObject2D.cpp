#include "RenderObject2D.h"

RenderObject2D::RenderObject2D(const glm::vec2 & position, const glm::vec2 & size, const glm::vec2 & rotation, const glm::vec4 color)
	: m_Position(position), m_Rotation(rotation), m_Size(size), m_Color(color)
{

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/vertex.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment.frag" },
		{ GL_NONE, NULL }
	};

	const GLfloat vertices[] =
	{
		0.0f	, 0.0f		, 0.0f, 1.0f,
		m_Size.x, 0.0f		, 0.0f, 1.0f,
		m_Size.x, m_Size.y	, 0.0f, 1.0f,
		0.0f	, m_Size.y	, 0.0f, 1.0f
	};

	const GLfloat colors[] =
	{
		m_Color.r, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b, 1.0f
	};

	static const GLushort indicies[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	VertexData data =
	{
		vertices, sizeof(vertices),
		colors, sizeof(colors),
		indicies, sizeof(indicies),
		GL_TRIANGLES, 6
	};
	
	RenderObject::init(shaders, data);
}
//
//RenderObject2D::RenderObject2D(ShaderInfo* shaders, VertexData vert, Properties2D prop)
//	: m_Properties(prop), m_Mode(vert.mode)
//{
//	if (init(shaders, vert))
//	{
//		m_Indices = (vert.viSize / sizeof(*vert.eIndices));
//	}
//
//}

void RenderObject2D::updateUniforms()
{
	glm::mat4 worldMatrix = glm::translate(glm::vec3(m_Position.x, m_Position.y, 0.0f));
	setUniformMat4("World", worldMatrix);
}


