#include "LightBill.h"

LightBill::LightBill(float x, float y, float z)
	: m_Pos(glm::vec3(x,y,z))
{
	_init();
	_initShaders();

	m_pShader->setUniform3f("BillboardPos", m_Pos);
	m_pShader->setUniform2f("BillboardSize", glm::vec2(1.0f, 0.125f));
}

LightBill::~LightBill()
{
	delete m_pIndexBuffer;
	delete m_pShader;
}

glm::vec3 LightBill::getPosition() const
{
	return m_Pos;
}

Shader * LightBill::getShader()
{
	return m_pShader;
}

void LightBill::move(glm::vec3 move)
{
	m_Pos = move;
}

void LightBill::draw(Camera camera, glm::mat4 projection)

{

	m_pShader->bind();
	glm::mat4 ViewMatrix = camera.getViewMatrix();

	glm::vec3 cameraRight_worldspace = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glm::vec3 cameraUp_worldspace = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	//
	m_pShader->setUniform3f("CameraRightWorld", cameraRight_worldspace);
	m_pShader->setUniform3f("CameraUpWorld", cameraUp_worldspace);

	glm::mat4 ViewProjMatrix = projection * ViewMatrix;
	m_pShader->setUniformMat4f("ViewProj", ViewProjMatrix);
	m_Vao.bind();
	m_pIndexBuffer->bind();
	m_pShader->setUniform3f("BillboardPos", m_Pos);
	GLCall(glDrawElements(GL_TRIANGLES, m_pIndexBuffer->getIndices(), GL_UNSIGNED_SHORT, 0));
}

void LightBill::_init()
{
	static const GLfloat vertices[] = {
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	static const unsigned short indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_Vao.addBuffer(new Buffer(vertices, sizeof(vertices), 3), 0);
	m_pIndexBuffer = new IndexBuffer(indices, sizeof(indices));
}

void LightBill::_initShaders()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/lightBillVert.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/lightBillFrag.glsl" },
		{ GL_NONE, NULL}
	};
	m_pShader = new Shader(shaders);
}


