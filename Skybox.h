#ifndef SKYBOX_H
#define SKYBOX_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"

class Skybox
{
private:
	VertexArray m_Vao;
	IndexBuffer* m_pIndexBuffer;
	Shader* m_pShader;
	GLuint m_Texture;
public:
	Skybox(const char* skybox, GLenum texture);
	~Skybox();

	void render(const Camera& camera);

private:
	void _initBox();
	void _initTexture(const char* skyboxPath, GLenum texture);
	void _initShaders();
};

#endif
