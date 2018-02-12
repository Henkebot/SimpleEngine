#ifndef PICKING_TEXTURE_H
#define PICKING_TEXTURE_H

#include <GL\glew.h>

class PickingTexture
{
public:
	struct PixelInfo
	{
		float objectID;
		float drawID;
		float primID;
	};
private:
	GLuint m_Fbo;
	GLuint m_PickingTexture;
	GLuint m_DepthTexture;
public:
	PickingTexture();

	bool init(GLuint width, GLuint height);

	void enableWriting();
	void disableWriting();

	PixelInfo readPixel(GLuint x, GLuint y);
};

#endif // !PICKING_TEXTURE_H

