#include "Buffer.h"

Buffer::Buffer(const GLfloat * data, GLint count, GLuint components)
	:m_Components(components)
{
	GLCall(glGenBuffers(1, &m_Buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW));

}

Buffer::~Buffer()
{
	GLCall(glDeleteBuffers(1, &m_Buffer));
}

GLuint Buffer::getCompCount() const
{
	return m_Components;
}

void Buffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Buffer));
}

void Buffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
