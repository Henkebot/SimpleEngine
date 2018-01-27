#include "TextureCube.h"

TextureCube::TextureCube(float x, float y, float z, const GLchar * texture)
{
	setPosition(x, y, z);
	setSize(1, 1, 1);


	static ShaderInfo cubeTexShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/cubeTexVert.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/cubeTexFrag.glsl" },
		{ GL_NONE, NULL }
	};

	_initShaders(cubeTexShaders);
	_initTextureCube();
	
	m_Texture = new Texture(texture);

}

TextureCube::~TextureCube()
{
	delete m_Texture;
}

void TextureCube::prepare()
{
	m_Texture->bind(GL_TEXTURE0);
	m_pShaderProgram->bind();
	m_VertexArray.bind();
	m_IndexBuffer->bind();

	m_pShaderProgram->setUniformMat4f("World", m_Translation * m_Scale);
}

void TextureCube::unbind()
{
	m_pShaderProgram->unbind();
	m_VertexArray.unbind();
	m_IndexBuffer->unbind();
	m_Texture->unbind();
}

void TextureCube::_initTextureCube()
{

	static const GLfloat vertices[] =
	{
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f,  -1.0f, -1.0f,
		1.0f,   1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		1.0f, 1.0f,  1.0f,
		-1.0f, 1.0f,  1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f

	};

	m_VertexArray.addBuffer(new Buffer(vertices, sizeof(vertices), 3), 0);

	static const GLfloat uv[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		
	};

	m_VertexArray.addBuffer(new Buffer(uv, sizeof(uv), 2), 1);

	static const GLushort indices[] =
	{
		0,  1,  2,  2,  3,  0,   //front

		4,  5,  6,  4,  6,  7,   //right

		8,  9,  10, 8,  10, 11 ,  //back

		12, 13, 14, 12, 14, 15,  //left

		16, 17, 18, 16, 18, 19,  //upper

		20, 21, 22, 20, 22, 23   //bottom
	};

	m_IndexBuffer = new IndexBuffer(indices, (sizeof(indices) / sizeof(GLushort)));


	

}
