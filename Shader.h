#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <glm.hpp>
#include <stdio.h>
#include "OpenGLError.h"


struct ShaderInfo
{
	GLenum type;
	const char* fileName;
	GLuint shader;
};

class Shader
{
private:
	GLuint m_Program;

public:
	Shader(ShaderInfo* shaders);
	~Shader();
	
	void setUniformMat4f(const GLchar* uniform, glm::mat4 mat);

	void bind();
private:
	const GLchar* readShader(const char* fileName);
	GLuint loadShaders(ShaderInfo* shaders);
};



#endif


