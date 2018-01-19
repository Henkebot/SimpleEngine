#include "Entity2D.h"

RenderObject2D::RenderObject2D(Properties2D properties)
	: m_Properties(properties)
{
	ShaderInfo shaders[] = 
	{
		{ GL_VERTEX_SHADER, "shaders/vertex.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragment.frag" },
		{ GL_NONE, NULL }
	};

	const GLfloat vertices[] =
	{
		0.0f				, 0.0f				, 0.0f, 1.0f,
		properties.size.x	, 0.0f				, 0.0f, 1.0f,
		properties.size.x	, properties.size.y	, 0.0f, 1.0f,
		0.0f				, properties.size.y	, 0.0f, 1.0f
	};

	const GLfloat colors[] =
	{
		properties.color.r, properties.color.g, properties.color.b, 1.0f,
		properties.color.r, properties.color.g, properties.color.b, 1.0f,
		properties.color.r, properties.color.g, properties.color.b, 1.0f,
		properties.color.r, properties.color.g, properties.color.b, 1.0f
	};

	static const GLushort indicies[] =
	{
		0, 1, 2, 
		2, 3, 0
	};

	VertexData data = 
	{
		vertices, sizeof(vertices),
		colors, sizeof(colors),
		indicies, sizeof(indicies),
		GL_TRIANGLES, 6
	};

	m_Mode = GL_TRIANGLES;
	m_Indices = 6;

	init(shaders, data);
}

RenderObject2D::RenderObject2D(ShaderInfo* shaders, VertexData vert, Properties2D prop)
	: m_Properties(prop), m_Mode(vert.mode)
{
	if (init(shaders, vert))
	{
		m_Indices = (vert.viSize / sizeof(*vert.eIndices));
	}

}

void RenderObject2D::prepare() const
{
	glUseProgram(m_Program);
	// ALL THE UNIFORMS HERE
	for (Uniform uni : m_Uniforms)
	{
		GLint location = glGetUniformLocation(m_Program, uni.name);
		switch (uni.type)
		{
		case Vec2:
			{
				glUniform2fv(location, 1, &uni.vec2[0]);
			}break;
		case Mat4:
			{
				glUniformMatrix4fv(location, 1, GL_FALSE, &uni.mat4[0][0]);
			}break;
		}
	}

	glBindVertexArray(m_Vao);
}

void RenderObject2D::unbind()
{
	glUseProgram(0);
	glBindVertexArray(0);
	
	m_Uniforms.clear();
}

GLenum RenderObject2D::getMode() const
{
	return m_Mode;
}

GLuint RenderObject2D::getIndicies() const
{
	return m_Indices;
}

void RenderObject2D::setUniform2f(const GLchar * name, glm::vec2 vector)
{
	Uniform uni;
	uni.type = Vec2;
	uni.vec2 = vector;
	uni.name = name;

	m_Uniforms.push_back(uni);
}

void RenderObject2D::setUniformMat4(const GLchar * name, glm::mat4 matrix)
{
	Uniform uni;
	uni.type = Mat4;
	uni.mat4 = matrix;
	uni.name = name;

	m_Uniforms.push_back(uni);
}

bool RenderObject2D::init(ShaderInfo* shaders, const VertexData& vert)
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
	glBindVertexArray(0);

	return true;
}

