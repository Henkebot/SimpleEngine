#ifndef LIGHT_BILL_H
#define LIGHT_BILL_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class LightBill
{
private:
	VertexArray m_Vao;
	IndexBuffer* m_pIndexBuffer;
	Shader* m_pShader;
	glm::vec3 m_Pos;
public:
	LightBill(float x, float y, float z);
	~LightBill();

	glm::vec3 getPosition() const;
	Shader* getShader();
	void move(glm::vec3);
	void draw();

private:
	void _init();
	void _initShaders();
};	


#endif


