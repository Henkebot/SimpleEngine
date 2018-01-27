#ifndef CUBE_H
#define CUBE_H

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"


class Cube
{
private:
	Shader* m_pShaderProgram;
	VertexArray m_VertexArray;
	IndexBuffer* m_IndexBuffer;

	glm::mat4 m_Translation;
public:
	Cube(float xPos, float yPos, float zPos);
	~Cube();

	void setCamMatrix(const Camera& camera);

	GLuint getIndices() const;
	void prepare();

private:
	void _buildCube();

};

#endif
