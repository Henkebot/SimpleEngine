#include <crtdbg.h>

#include "window.h"
#include "Render2D.h"
#define WIDTH 1280.0f
#define HEIGHT 720.0f
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window window("Test", WIDTH, HEIGHT);
	Render2D render;

	RenderObject2D test({glm::vec2(-1.0f,0), glm::vec2(0,0), glm::vec2(1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f)});

	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);

		static const GLfloat aspect = WIDTH / HEIGHT;
		static GLfloat viewDistance = 1.0f;
		static GLfloat angle = 0.0f;
		angle += 0.001f;
		static glm::mat4 ortho = glm::ortho(-aspect * viewDistance, aspect* viewDistance, -viewDistance, viewDistance, -1000.0f, 1000.0f);
		//static glm::mat4 ortho = glm::perspective(70.0f, aspect, 1.0f, 200.0f);
		static glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -2.0f));
		glm::mat4 rotation = glm::rotate(angle, glm::vec3(0, 0, 1));
		static float halfX = 1280.0f / 2.0f;
		static float halfY = 720.0f / 2.0f;
		test.setUniform2f("light_pos", glm::vec2((float)((x / 1280.0f) * 2.0f - 1.0f), (float)(1.0f - (y / 720.0f) * 2.0f)));

		test.setUniformMat4("Projection", ortho);
		test.setUniformMat4("Model", translation * rotation);

		render.submit(&test);

		render.flush();
		window.update();
	}
	
	return 0;
}