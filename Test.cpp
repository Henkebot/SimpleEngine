#include "window.h"

#include "Cube.h"
#include "TextureCube.h"
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
	Cube* textureCubes[1];
	textureCubes[0] = new TextureCube(0, 0, 0,"res/sky.bmp");
	textureCubes[0]->setSize(200, 200, 200);
	
	Cube* cubes[25];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cubes[i + (j * 5)] = new Cube(i * 2, 2, j * 2);
		}
	}

	
	
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

		for (auto& cube : textureCubes)
		{
			cube->setCamMatrix(camera);
			render.submit(cube);
		}

		render.flush();
		window.update();
	}

	for (auto& cube : cubes)
		delete cube;

	for (auto& cube : textureCubes)
		delete cube;

	return 0;
};

