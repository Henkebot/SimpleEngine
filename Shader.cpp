#include "Shader.h"
#include <iostream>

const GLchar * Shader::readShader(const char * fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "rb");

	if (!file)
	{
		std::cout << "Shader: Couldn't open " << fileName << ".\n";
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	GLchar* source = new GLchar[length + 1];
	fread(source, 1, length, file);
	fclose(file);
	source[length] = '\0';

	return const_cast<const GLchar*>(source);
}

GLuint Shader::loadShaders(ShaderInfo * shaders)
{
	if (shaders == nullptr) return 0;

	GLuint program; 
	GLCall(program = glCreateProgram());

	ShaderInfo* entry = shaders;

	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;

		const GLchar* shaderSource = readShader(entry->fileName);
		if (shaderSource == nullptr)
		{
			GLCall(glDeleteShader(entry->shader));
			entry->shader = 0;
			// Error notified in readShader()
			// Clear all the shaders since an error occured
			for (entry = shaders; entry->type != GL_NONE; entry++)
			{
				GLCall(glDeleteShader(entry->shader));
				entry->shader = 0;
			}

			return 0;
		}
		// Wanna do nullptr instead of NULL
		GLCall(glShaderSource(shader, 1, &shaderSource, NULL));
		delete[] shaderSource;

		GLCall(glCompileShader(shader));
		GLint compiledFlag;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledFlag));

		if (!compiledFlag)
		{
			GLsizei length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(shader, length, &length, log);
			std::cout << entry->fileName << " failed, log:\n" << log << std::endl;
			delete[] log;


			return 0;
		}

		glAttachShader(program, shader);
		entry++;
	}

	GLCall(glLinkProgram(program));

	GLint linkFlag;
	glGetProgramiv(program, GL_LINK_STATUS, &linkFlag);
	if (!linkFlag)
	{
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, &length, log);
		
		std::cout << "Failed to link program(" << shaders->fileName << ", log:\n" << log << std::endl;

		for (entry = shaders; entry->type != GL_NONE; entry++)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		return 0;
	}
	// Detach and remove shaders because they are no longer needed.
	for (entry = shaders; entry->type != GL_NONE; entry++)
	{
		glDetachShader(m_Program, entry->shader);
		glDeleteShader(entry->shader);
		entry->shader = 0;
	}

	return program;
}

Shader::Shader(ShaderInfo * shaders)
{
	m_Program = loadShaders(shaders);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_Program));
	GLCall(glUseProgram(0));
}

void Shader::setUniformMat4f(const GLchar * uniform, glm::mat4 mat)
{
	bind();

	GLint location;
	GLCall(location = glGetUniformLocation(m_Program, uniform));
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]));

	//unbind();
}

void Shader::bind()
{
	GLCall(glUseProgram(m_Program));
}

void Shader::unbind()
{
	GLCall(glUseProgram(0));
}

const GLuint Shader::getProgram() const
{
	return m_Program;
}
