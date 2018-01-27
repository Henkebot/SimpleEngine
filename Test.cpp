#include "window.h"

#include "Cube.h"
#include "Render2D.h"
#include "Camera.h"

main
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//----Create Window
	Window window("Test",1280,720);
	window.hideAndGrabMouseMode();
	
	//----OpenGL Settings
	GLCall(glEnable(GL_DEPTH_TEST));
	
	//----Crate cubes
	Cube* cubes[25];
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			cubes[i+(j*5)] = new Cube(i, 0, j);
	
	//----Camera
	Camera camera(window.getWidth(), window.getHeight());

	//---Setup Render
	SimpleRender render;

	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);

		camera.update(x,y);
		for (auto& cube : cubes)
		{
			cube->setCamMatrix(camera);
			render.submit(cube);
		}

		render.flush();
		window.update();
	}

	for (auto& cube : cubes)
		delete cube;

	return 0;
};

