#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL\glew.h>
#include "Buffer.h"
#include <vector>

class VertexArray
{
private:
	GLuint m_Vertex;
	std::vector<Buffer*> m_Buffers;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(Buffer* buffer, GLuint index);
	void bind() const;
	void unbind() const;

};


#endif // !VERTEX_BUFFER_H

