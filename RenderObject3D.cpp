#include "RenderObject3D.h"

RenderObject3D::RenderObject3D(const glm::vec3 & position, const glm::vec3 & size, const glm::vec3& color)
	:m_Position(position), m_Color(color), m_Rotation(glm::mat4(1)), m_Translation(glm::translate(position))
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/vert3D.vert" },
		{ GL_GEOMETRY_SHADER, "shaders/geo3D.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment3D.frag" },
		{ GL_NONE, NULL }
	};

	const GLfloat vertices[] =
	{
		 0.5f,  0.5f,  0.5f, 1.0f, // 0
		 0.5f,  0.5f, -0.5f, 1.0f, // 1
		-0.5f,  0.5f, -0.5f, 1.0f,// 2
		-0.5f,  0.5f,  0.5f, 1.0f,// 3

		-0.5f, -0.5f, -0.5f, 1.0f,// 4
		 0.5f, -0.5f, -0.5f, 1.0f,// 5
		-0.5f, -0.5f, 0.5f, 1.0f,// 6
		 0.5f, -0.5f, 0.5f, 1.0f,// 7
	};

	const GLushort indicies[] =
	{
		0, 1, 2, 
		2, 3, 0,

		5, 1, 0,
		0, 7, 5,

		4, 2, 1,
		1, 5, 4,

		6, 3, 2,
		2, 4, 6,

		5, 7, 6,
		6, 4, 5,

		7, 0, 3,
		3, 6, 7
	};

	const GLfloat colors[] =
	{
		m_Color.r / 2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g/2, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b/2, 1.0f,
		m_Color.r/2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g/2, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b/2, 1.0f,
		m_Color.r/2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g/2, m_Color.b, 1.0f
	};

	VertexData data =
	{
		vertices, sizeof(vertices),
		colors, sizeof(colors),
		indicies, sizeof(indicies),
		GL_TRIANGLES, (6*6)
	};

	m_Size = glm::scale(glm::mat4(1), size);
	translate(m_Position);

	RenderObject::init(shaders, data);
}

void RenderObject3D::updateUniforms()
{
	setUniformMat4("World", m_Translation);
	setUniformMat4("World", m_Rotation);
	setUniformMat4("World", m_Size);
}

glm::vec3 RenderObject3D::getPos3fv() const
{
	glm::vec4 result = m_Translation *  m_Size * m_Rotation * glm::vec4(m_Position,1.0f);
	return glm::vec3(result.x,result.y,result.z);
}

void RenderObject3D::rotate(float angle, const glm::vec3 & axis)
{
	m_Rotation = glm::rotate(angle, axis) *  m_Translation;
	glm::vec4 result = m_Rotation * glm::vec4(m_Position, 1.0f);
	//m_Position = glm::vec3(result.x, result.y, result.z);
}

void RenderObject3D::translate(const glm::vec3 & vector)
{
	m_Translation = glm::translate(vector);
	glm::vec4 result = m_Translation * glm::vec4(m_Position, 1.0f);
	//m_Position = glm::vec3(result.x, result.y, result.z);
}
