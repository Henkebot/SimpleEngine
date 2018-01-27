#ifndef RENDER_2D_H
#define RENDER_2D_H
#include <deque>
#include <GL\glew.h>
//#include "RenderObject.h"
#include "Cube.h"
#include "OpenGLError.h"

class SimpleRender
{
private:
//	std::deque<RenderObject*> m_RenderQueue;
	std::deque<Cube*> m_RenderQueueCubes;
public:
//	void submit(RenderObject* er2D);
	void submit(Cube* cube);
	void flush();
};
#endif