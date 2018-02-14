#include "Texture.h"

Texture::Texture(const GLchar * textureSrc, const GLchar* name, TYPE type)
{
	if(type == BMP)
		m_Texture = _initBMP(textureSrc);
	else if(type == PNG)
		m_Texture = _initPNG(textureSrc);
	m_Name = name;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_Texture));
}

void Texture::bind(Shader* shaderProgram, const int textureNr)
{
	GLuint textureId;
	GLCall(textureId = glGetUniformLocation(shaderProgram->getProgram(), m_Name))
	GLCall(glActiveTexture(GL_TEXTURE0 + textureNr));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Texture));
	GLCall(glUniform1i(textureId, textureNr));
}

void Texture::unbind()
{
	glActiveTexture(GL_TEXTURE0);
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

GLuint Texture::_initBMP(const GLchar * textureSrc)
{
	std::cout << "\nTexture: Reading BMP(" << textureSrc << ")...";

	unsigned char header[54];	// Each BMP file begins by a 54-bytes header
	unsigned int dataPos;		// Position in the file where the actual data begins
	unsigned int width, height;	
	unsigned int imageSize;		// = width * height * 3

	unsigned char* data;	// actuall RGB data

	FILE* file;
	fopen_s(&file, textureSrc, "rb");
	if (!file)
	{
		std::cout << "Error: Couldnt open (" << textureSrc << ")\n";
		return 0;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << "Error: Not a correct BMP file (" << textureSrc << ")\n";
		return 0;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "Error: Not a correct BMP file (" << textureSrc << ")\n";
	}

	dataPos		= *(int*)&(header[0x0A]);
	imageSize	= *(int*)&(header[0x22]);
	width		= *(int*)&(header[0x12]);
	height		= *(int*)&(header[0x16]);

	// Sometimes the BMP files are misformatted, time to guess information!
	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;
	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);
	std::cout << "Done!\n";
	GLuint texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));


	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	delete data;
	return texture;

}

GLuint Texture::_initPNG(const GLchar * textureSrc)
{
	std::cout << "\tReading png (" << textureSrc << ")...";
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned error = lodepng::decode(image, width, height, textureSrc);
	if (error != 0)
	{
		std::cout << "\nerror " << error << ": " << lodepng_error_text(error) << std::endl;
	}
	GLuint texture;
	GLCall(glGenTextures(1, &texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture));


	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	std::cout << "Done!\n";
	return texture;
}
