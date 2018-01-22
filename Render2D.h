#ifndef RENDER_2D_H
#define RENDER_2D_H
#include <deque>
#include <GL\glew.h>
#include "RenderObject.h"

class Render2D
{
private:
	std::deque<RenderObject*> m_RenderQueue;
public:
	void submit(RenderObject* er2D);
	void flush();
};
#endif