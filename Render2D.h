#ifndef RENDER_2D_H
#define RENDER_2D_H
#include <deque>
#include <GL\glew.h>
#include "Entity2D.h"

class Render2D
{
private:
	std::deque<Entity2D*> m_RenderQueue;
	GLfloat m_ScreenWidth;
	GLfloat m_ScreenHeight;
public:
	Render2D(int width, int height);
	void submit(Entity2D* er2D);
	void flush(double x, double y);
};
#endif