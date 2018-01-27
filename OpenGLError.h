#ifndef OPENGL_ERROR_H
#define OPENGL_ERROR_H

#include <iostream>
#include <GL\glew.h>


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GlClearError(); \
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__))

static void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

#endif // !OPENGL_ERROR_H

