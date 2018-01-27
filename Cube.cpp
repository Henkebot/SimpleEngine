#include "Cube.h"

Cube::Cube(float xPos, float yPos, float zPos)
{
	_initShaders();
	_buildCube();
	setPosition(xPos, yPos, zPos);
	setSize(1, 1, 1);
}

Cube::Cube(float xPos, float yPos, float zPos, ShaderInfo * shaders)
{
	_initShaders(shaders);
	_buildCube();

	setPosition(xPos, yPos, zPos);
	setSize(1, 1, 1);
}

Cube::Cube()
{

}

Cube::~Cube()
{
	delete m_pShaderProgram;
	delete m_IndexBuffer;
}

void Cube::setPosition(float xPos, float yPos, float zPos)
{
	m_Translation = glm::translate(glm::mat4(1), glm::vec3(xPos, yPos, zPos));
}

void Cube::setSize(float xSize, float ySize, float zSize)
{
	m_Scale = glm::scale(glm::vec3(xSize, ySize, zSize));	
}

void Cube::setCamMatrix(const Camera & camera)
{
	m_pShaderProgram->setUniformMat4f("View", camera.getViewMatrix());
}

GLuint Cube::getIndices() const
{
	return m_IndexBuffer->getIndices();
}

void Cube::prepare()
{
	m_pShaderProgram->bind();
	m_VertexArray.bind();
	m_IndexBuffer->bind();

	m_pShaderProgram->setUniformMat4f("World", m_Translation * m_Scale);
}

void Cube::unbind()
{
	m_pShaderProgram->unbind();
	m_VertexArray.unbind();
	m_IndexBuffer->unbind();
}

void Cube::_initShaders(ShaderInfo * shaders)
{
	static ShaderInfo tempShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/cubeVert.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/cubeFrag.glsl" },
		{ GL_NONE, NULL }
	};

	m_pShaderProgram = new Shader((shaders == nullptr) ? tempShaders : shaders) ;

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	m_pShaderProgram->setUniformMat4f("Projection", projection);

}

void Cube::_buildCube()
{
	static const GLfloat vertices[] =
	{
		0.5f,  0.5f,  0.5f, 1.0f, // 0
		0.5f,  0.5f, -0.5f, 1.0f, // 1
		-0.5f,  0.5f, -0.5f, 1.0f,// 2
		-0.5f,  0.5f,  0.5f, 1.0f,// 3

		-0.5f, -0.5f, -0.5f, 1.0f,// 4
		0.5f, -0.5f, -0.5f, 1.0f,// 5
		-0.5f, -0.5f, 0.5f, 1.0f,// 6
		0.5f, -0.5f, 0.5f, 1.0f,// 7
	};
	
	m_VertexArray.addBuffer(new Buffer(vertices, sizeof(vertices), 4), 0);

	static const GLushort indices[] =
	{
		0, 1, 2,
		2, 3, 0,

		5, 1, 0,
		0, 7, 5,

		4, 2, 1,
		1, 5, 4,

		6, 3, 2,
		2, 4, 6,

		5, 7, 6,
		6, 4, 5,

		7, 0, 3,
		3, 6, 7
	};
	m_IndexBuffer = new IndexBuffer(indices, (sizeof(indices) / sizeof(GLushort)));

	glm::vec3 m_Color(1.0f, 1.0f, 1.0f);

	static const GLfloat colors[] =
	{
		m_Color.r / 2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g / 2, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b / 2, 1.0f,
		m_Color.r / 2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g / 2, m_Color.b, 1.0f,
		m_Color.r, m_Color.g, m_Color.b / 2, 1.0f,
		m_Color.r / 2, m_Color.g, m_Color.b, 1.0f,
		m_Color.r, m_Color.g / 2, m_Color.b, 1.0f
	};

	m_VertexArray.addBuffer(new Buffer(colors, sizeof(colors), 4), 1);

}
