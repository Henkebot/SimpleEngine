#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

#if _DEBUG
#include <iostream>
#define DPRINT(text) std::cout << (text)
#elif
#define DPRINT(text)
#endif

struct ShaderInfo
{
	GLenum type;
	const char* fileName;
	GLuint shader;
};

const GLchar* readShader(const char* fileName);

GLuint loadShaders(ShaderInfo* shaders);

#endif


