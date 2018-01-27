#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include "OpenGLError.h"

class Texture
{
private:
	GLuint m_Texture;

public:
	Texture(const GLchar* textureSrc);
	~Texture();

	void bind(const GLenum& textureUnit);
	void unbind();

private:
	GLuint _initBMP(const GLchar* textureSrc);
};

#endif // !TEXTURE_H

