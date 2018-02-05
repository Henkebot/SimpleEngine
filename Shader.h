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
	ShaderInfo* m_Shader;

public:
	Shader(ShaderInfo* shaders);
	~Shader();

	void setUniformMat3f(const GLchar* uniform, glm::mat3 mat);
	void setUniformMat4f(const GLchar* uniform, glm::mat4 mat);
	void setUniform4f(const GLchar* uniform, glm::vec4 vec);
	void setUniform3f(const GLchar* uniform, glm::vec3 vector);
	void setUniform2f(const GLchar* uniform, glm::vec2 vector);
	void setUniform1f(const GLchar* uniform, float value);
	void setUniform1i(const GLchar* unifom, int value);


	void bind();
	void unbind();
	const GLuint getProgram() const;
private:
	const GLchar* readShader(const char* fileName);
	GLuint loadShaders(ShaderInfo* shaders);
};



#endif


