#include <crtdbg.h>

#include "window.h"
#include "Render2D.h"

#include "RenderObject2D.h"

#define WIDTH 1280.0f
#define HEIGHT 720.0f
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window window("Test", WIDTH, HEIGHT);
	Render2D render;

	//RenderObject2D test({glm::vec2(-1.0f,0), glm::vec2(0,0), glm::vec2(0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)});
	RenderObject* square = new RenderObject2D(glm::vec2(-1.0f, -1.0f), glm::vec2(2.0f,2.0f));

	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);

		static const GLfloat aspect = WIDTH / HEIGHT;
		static GLfloat viewDistance = 1.0f;
		static GLfloat angle = 0.0f;
		//angle += 0.001f;
		static glm::mat4 ortho = glm::ortho(-aspect * viewDistance, aspect* viewDistance, -viewDistance, viewDistance, -1000.0f, 1000.0f);
		//static glm::mat4 ortho = glm::perspective(70.0f, aspect, 1.0f, 200.0f);
		
		
		//square->setUniform2f("light_pos", glm::vec2((float)((x / 1280.0f) * 2.0f - 1.0f), (float)(1.0f - (y / 720.0f) * 2.0f)));
		x = (x / 1280.0f) * 16.0f - 8.0f;
		y = (4.5f - (y / 720.0f) * 9.0f);
		std::cout << "\rX: " << x << "Y : " << y << "\n";
		square->setUniform2f("light_pos", glm::vec2((float)(x), (float)(y)));
		square->setUniformMat4("Projection", ortho);
		square->updateUniforms();
		render.submit(square);

		render.flush();
		window.update();
	}
	
	return 0;
}