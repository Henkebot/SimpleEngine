#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H

#include "Cube.h"
#include "Texture.h"

class TextureCube : public Cube
{
private:
	Texture * m_Texture;

public:
	TextureCube(float x, float y, float z, const GLchar* texture);
	virtual ~TextureCube();

	void prepare() override;
	void unbind() override;

private:
	void _initTextureCube();
};

#endif // !TEXTURE_CUBE_H

