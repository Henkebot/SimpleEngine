#include "Render2D.h"

Render2D::Render2D(int width, int height)
	: m_ScreenWidth(width), m_ScreenHeight(height)
{
}

void Render2D::submit(Entity2D * er2D)
{
	m_RenderQueue.push_back(er2D);
}

void Render2D::flush(double x, double y)
{
	while (!m_RenderQueue.empty())
	{
		Entity2D* er2D = m_RenderQueue.front();
		
		er2D->bind();
		{
			static const GLfloat aspect = m_ScreenWidth / m_ScreenHeight;
			static GLfloat viewDistance = 1.0f;
			static GLfloat angle = 0.0f;
			//angle += 0.001f;
			//static glm::mat4 ortho = glm::ortho(-aspect * viewDistance, aspect* viewDistance, -viewDistance, viewDistance, -1000.0f, 1000.0f);
			static glm::mat4 ortho = glm::perspective(70.0f, aspect, 1.0f, 200.0f);
			static glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -2.0f));
			glm::mat4 rotation = glm::rotate(angle, glm::vec3(0,1,0));
			static float halfX = 1280.0f / 2.0f;
			static float halfY = 720.0f / 2.0f;
			er2D->setUniform2f("light_pos", glm::vec2((float)((x / 1280.0f) * 2.0f - 1.0f), (float)(1.0f - (y / 720.0f) * 2.0f)));
			
			er2D->setUniformMat4("Projection", ortho);
			er2D->setUniformMat4("Model", translation * rotation);
			glDrawElements(er2D->getMode(), er2D->getIndicies(), GL_UNSIGNED_SHORT, NULL);
		}
		er2D->unbind();

		m_RenderQueue.pop_front();
	}
}
