#include "Cube.h"

Cube::Cube(float xPos, float yPos, float zPos)
{
	m_Translation = glm::translate(glm::mat4(1), glm::vec3(xPos, yPos, zPos));
	
	_buildCube();
}

Cube::~Cube()
{
	delete m_pShaderProgram;
	delete m_IndexBuffer;
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

	m_pShaderProgram->setUniformMat4f("Translation", m_Translation);
}

void Cube::_buildCube()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/cubeVert.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/cubeFrag.glsl" },
		{ GL_NONE, NULL }
	};

	m_pShaderProgram = new Shader(shaders);

	static const GLfloat aspect = 1280.0f / 720.0f;
	static glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);
	m_pShaderProgram->setUniformMat4f("Projection", projection);

	const GLfloat vertices[] =
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

	const GLushort indices[] =
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

	const GLfloat colors[] =
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
