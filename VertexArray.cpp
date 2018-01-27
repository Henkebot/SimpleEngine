#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_Vertex));
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Vertex);
	for (auto& buffer : m_Buffers)
		delete buffer;
}

void VertexArray::addBuffer(Buffer * buffer, GLuint index)
{
	bind();
	buffer->bind();

	GLCall(glEnableVertexAttribArray(index));
	GLCall(glVertexAttribPointer(index, buffer->getCompCount(), GL_FLOAT, GL_FALSE, 0, 0));

	buffer->unbind();
	unbind();
	m_Buffers.push_back(buffer);
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_Vertex));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}
