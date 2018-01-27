#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <GL\glew.h>
#include "OpenGLError.h"

class IndexBuffer
{
private:
	GLuint m_ElementBuffer;
	GLuint m_Indices;
public:
	IndexBuffer(const GLushort* indices, GLuint count);
	~IndexBuffer();
	
	void bind();
	void unbind();

	GLuint getIndices() const;
};

#endif // !INDEX_BUFFER_H

