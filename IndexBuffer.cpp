#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLushort * indices, GLuint count)
	: m_Indices(count)
{
	GLCall(glGenBuffers(1, &m_ElementBuffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer));
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * count, indices, GL_STATIC_DRAW));
	}
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_ElementBuffer));
}

void IndexBuffer::bind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer));
}

void IndexBuffer::unbind()
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLuint IndexBuffer::getIndices() const
{
	return m_Indices;
}
