#include "Render2D.h"

//void SimpleRender::submit(RenderObject * er2D)
//{
//	m_RenderQueue.push_back(er2D);
//}

void SimpleRender::submit(Cube * cube)
{
	m_RenderQueueCubes.push_back(cube);
}

void SimpleRender::flush()
{
	/*while (!m_RenderQueue.empty())
	{
		RenderObject* er2D = m_RenderQueue.front();
		
		er2D->prepare();
		{
			glDrawElements(er2D->getMode(), er2D->getIndicies(), GL_UNSIGNED_SHORT, NULL);
		}
		er2D->unbind();

		m_RenderQueue.pop_front();
	}*/
	while (!m_RenderQueueCubes.empty())
	{
		Cube* cube = m_RenderQueueCubes.front();
		
		cube->prepare();
			GLCall(glDrawElements(GL_TRIANGLES, cube->getIndices(), GL_UNSIGNED_SHORT, NULL));
		cube->unbind();

		m_RenderQueueCubes.pop_front();
		
	}
}
