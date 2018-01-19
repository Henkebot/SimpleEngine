#include "Entity2D.h"

Entity2D::Entity2D(Properties2D properties)
	: m_Properties(properties)
{
	ShaderInfo shaders[] = 
	{
		{ GL_VERTEX_SHADER, "shaders/vertex.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment.frag" },
		{ GL_NONE, NULL }
	};

	m_Program = loadShaders(shaders);
	
	//Remove comment if fuck up
	//glUseProgram(m_Program);

}

Entity2D::Entity2D(ShaderInfo* shaders, VertexData vert, Properties2D prop)
	: m_Properties(prop), m_Mode(vert.mode)
{
	if (init(shaders, vert))
	{
		m_Indices = (vert.viSize / sizeof(*vert.eIndices));
	}

}

void Entity2D::bind() const
{
	glUseProgram(m_Program);
	// ALL THE UNIFORMS HERE
	glBindVertexArray(m_Vao);
}

void Entity2D::unbind() const
{
	glUseProgram(0);
	glBindVertexArray(0);
}

GLenum Entity2D::getMode() const
{
	return m_Mode;
}

GLuint Entity2D::getIndicies() const
{
	return m_Indices;
}

bool Entity2D::init(ShaderInfo* shaders, const VertexData& vert)
{
	m_Program = loadShaders(shaders);
	
	if (!m_Program)
	{
		DPRINT("Failed to create program");
		return false;
	}
	// Remove comment if fuckup
	//glUseProgram(m_Program);

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);
	{
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vert.viSize, vert.eIndices, GL_STATIC_DRAW);
		}

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, vert.vpSize + vert.vcSize, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vert.vpSize, vert.vPositions);
			glBufferSubData(GL_ARRAY_BUFFER, vert.vpSize, vert.vcSize, vert.vColors);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vert.vpSize));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		}

	}
	//glBindVertexArray(0);

	return true;
}
