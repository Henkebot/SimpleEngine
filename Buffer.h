#ifndef BUFFER_H
#define BUFFER_H

#include <GL\glew.h>
#include "OpenGLError.h"

class Buffer
{
private:
	GLuint m_Buffer;
	GLuint m_Components;
public:
	Buffer(const GLfloat* data, GLint count, GLuint components);
	~Buffer();

	void bind() const;
	void unbind() const;
	GLuint getCompCount() const;
};


#endif
