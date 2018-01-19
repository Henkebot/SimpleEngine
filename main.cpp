#include <crtdbg.h>

#include "window.h"
#include "Render2D.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window window("Test", 1280, 720);
	Render2D render;

	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
	};

	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
	};

	static const GLushort cube_indices[] =
	{
		0, 1, 2
	};

	VertexData data =
	{
		cube_positions, sizeof(cube_positions),
		cube_colors, sizeof(cube_colors),
		cube_indices, sizeof(cube_indices),
		GL_TRIANGLES
	};

	Properties2D prop = { };

	Entity2D test(prop);

	while (window.closed())
	{
		window.clear();
		render.submit(&test);
		render.flush();
		window.update();
	}
	
	return 0;
}