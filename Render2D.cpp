#include "Render2D.h"

void Render2D::submit(const Entity2D * er2D)
{
	m_RenderQueue.push_back(er2D);
}

void Render2D::flush()
{
	while (!m_RenderQueue.empty())
	{
		const Entity2D* er2D = m_RenderQueue.front();
		
		er2D->bind();
		{
			glDrawElements(er2D->getMode(), er2D->getIndicies(), GL_UNSIGNED_SHORT, NULL);
		}
		er2D->unbind();

		m_RenderQueue.pop_front();
	}
}
