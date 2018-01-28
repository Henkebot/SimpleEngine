#include "Buffer.h"

Buffer::Buffer(const GLfloat * data, GLint count, GLuint components)
	:m_Components(components)
{
	GLCall(glGenBuffers(1, &m_Buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW));

}

Buffer::Buffer(const std::vector<glm::vec3> data)
	:m_Components(3)
{
	GLCall(glGenBuffers(1, &m_Buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[0], GL_STATIC_DRAW));
}

Buffer::Buffer(const std::vector<glm::vec2> data)
	: m_Components(2)
{
	GLCall(glGenBuffers(1, &m_Buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec2), &data[0], GL_STATIC_DRAW));
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
