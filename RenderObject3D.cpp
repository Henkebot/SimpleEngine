#include "RenderObject3D.h"

RenderObject3D::RenderObject3D(const glm::vec3 & position, const glm::vec3 & size, const glm::vec3& color)
	:m_Position(position), m_Size(size), m_Color(color), m_Rotation(glm::mat4(1)), m_Translation(glm::translate(position))
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/vert3D.vert" },
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

		1, 5, 4,
		4, 2, 1,

		2, 4, 6,
		6, 3, 2,

		1, 5, 7,
		7, 0, 1,

		3, 0, 6,
		6, 7, 0,

		4, 5, 7,
		7, 6, 4
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

	RenderObject::init(shaders, data);
}

void RenderObject3D::updateUniforms()
{
	setUniformMat4("World", m_Translation);
	setUniformMat4("World", m_Rotation);
}

glm::vec3 RenderObject3D::getPos3fv() const
{
	return m_Position;
}

void RenderObject3D::rotate(float angle, const glm::vec3 & axis)
{
	m_Rotation = glm::rotate(angle, axis);
}

void RenderObject3D::translate(const glm::vec3 & vector)
{
	m_Position += vector;
	m_Translation = glm::translate(vector);
}
