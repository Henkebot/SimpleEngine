#include <crtdbg.h>

#include "window.h"
#include "Render2D.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window window("Test", 1280, 720);
	Render2D render(1280,720);

	Properties2D prop = { };

	Entity2D test({glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)});
	Entity2D test2({ glm::vec2(0,0), glm::vec2(0,0), glm::vec2(-0.5f, -0.5f), glm::vec3(0.2f, 0.2f, 0.2f) });
	Entity2D test3({ glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0.5f, -0.5f), glm::vec3(0.7f, 0.7f, 0.7f) });
	Entity2D test4({ glm::vec2(0,0), glm::vec2(0,0), glm::vec2(-0.5f, 0.5f), glm::vec3(0.9f, 0.9f, 0.9f) });

	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		


		render.submit(&test);
		render.submit(&test2);
		render.submit(&test3);
		render.submit(&test4);

		render.flush(x, y);
		window.update();
	}
	
	return 0;
}