#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include "Shader.h"
#include "OpenGLError.h"
#include "lodepng.h"
enum TYPE : char
{
	PNG,
	BMP
};
class Texture
{
private:
	GLuint m_Texture;
	const GLchar* m_Name;

public:
	Texture(const GLchar * textureSrc, const GLchar* name, TYPE type);
	~Texture();

	void bind(Shader* shader, const int textureNr);
	void unbind();

private:
	GLuint _initBMP(const GLchar* textureSrc);
	GLuint _initPNG(const GLchar* textureSrc);
};

#endif // !TEXTURE_H

