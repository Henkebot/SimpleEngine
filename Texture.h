#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include "Shader.h"
#include "OpenGLError.h"

class Texture
{
private:
	GLuint m_Texture;
	const GLchar* m_Name;

public:
	Texture(const GLchar * textureSrc, const GLchar* name);
	~Texture();

	void bind(Shader* shader, const int textureNr);
	void unbind();

private:
	GLuint _initBMP(const GLchar* textureSrc);
};

#endif // !TEXTURE_H

