#ifndef RENDER_2D_H
#define RENDER_2D_H
#include <deque>
#include <GL\glew.h>
#include "Entity2D.h"

class Render2D
{
private:
	std::deque<const Entity2D*> m_RenderQueue;
public:
	void submit(const Entity2D* er2D);
	void flush();
};
#endif