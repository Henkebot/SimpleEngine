#include "Shader.h"

const GLchar * readShader(const char * fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "rb");

	if (!file)
	{
		DPRINT("Couldn't open shader file");
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

GLuint loadShaders(ShaderInfo * shaders)
{
	if (shaders == nullptr) return 0;

	GLuint program = glCreateProgram();

	ShaderInfo* entry = shaders;

	while (entry->type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->type);
		entry->shader = shader;

		const GLchar* shaderSource = readShader(entry->fileName);
		if (shaderSource == nullptr)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
			// Clear all the shaders since an error occured
			for (entry = shaders; entry->type != GL_NONE; entry++)
			{
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}

			return 0;
		}
		// Wanna do nullptr instead of NULL
		glShaderSource(shader, 1, &shaderSource, NULL);
		delete[] shaderSource;

		glCompileShader(shader);
		GLint compiledFlag;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledFlag);

		if (!compiledFlag)
		{
#if _DEBUG
			GLsizei length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			GLchar* log = new GLchar[length + 1];
			glGetShaderInfoLog(shader, length, &length, log);
			DPRINT(log);
			delete[] log;
#endif

			return 0;
		}

		glAttachShader(program, shader);
		entry++;
	}

	glLinkProgram(program);

	GLint linkFlag;
	glGetProgramiv(program, GL_LINK_STATUS, &linkFlag);
	if (!linkFlag)
	{
#if _DEBUG
		GLsizei length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, &length, log);
		DPRINT(log);
#endif
		for (entry = shaders; entry->type != GL_NONE; entry++)
		{
			glDeleteShader(entry->shader);
			entry->shader = 0;
		}
		return 0;
	}

	return program;
}
