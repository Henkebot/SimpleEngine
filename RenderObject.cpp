#include "RenderObject.h"

GLenum RenderObject::getMode() const
{
	return m_Mode;
}

GLuint RenderObject::getIndicies() const
{
	return m_Indices;
}

void RenderObject::setUniform2f(const GLchar * name, glm::vec2 vector)
{
	Uniform uni;
	uni.type = Vec2;
	uni.vec2 = vector;
	uni.name = name;

	m_Uniforms.push_back(uni);
}

void RenderObject::setUniformMat4(const GLchar * name, glm::mat4 matrix)
{
	Uniform uni;
	uni.type = Mat4;
	uni.mat4 = matrix;
	uni.name = name;

	m_Uniforms.push_back(uni);
}

void RenderObject::prepare()
{
	glUseProgram(m_Program);

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

void RenderObject::unbind()
{
	glUseProgram(0);
	glBindVertexArray(0);

	m_Uniforms.clear();
}

bool RenderObject::init(ShaderInfo * shaders, const VertexData & vert)
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

	m_Mode = vert.mode;
	m_Indices = vert.indices;

	return true;
}
