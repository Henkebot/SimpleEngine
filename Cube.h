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
protected:
	Shader* m_pShaderProgram;
	VertexArray m_VertexArray;
	IndexBuffer* m_IndexBuffer;

	glm::mat4 m_Translation;
	glm::mat4 m_Scale;
	
	void _initShaders(ShaderInfo* shaders = nullptr);
public:
	Cube(float xPos, float yPos, float zPos);
	Cube(float xPos, float yPos, float zPos, ShaderInfo* shaders);
	Cube();
	virtual ~Cube();

	void setPosition(float xPos, float yPos, float zPos);
	void setSize(float xSize, float ySize, float zSize);
	void setCamMatrix(const Camera& camera);

	GLuint getIndices() const;
	virtual void prepare();
	virtual void unbind();

private:
	void _buildCube();

};

#endif
