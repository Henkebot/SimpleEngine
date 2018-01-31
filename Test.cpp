#include "window.h"

#include "Cube.h"
#include "TextureCube.h"
#include "Render2D.h"
#include "Camera.h"
#include "Model.h"

main
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//----Create Window
	Window window("Test",1280,720);
	window.hideAndGrabMouseMode();
	
	//----OpenGL Settings
	GLCall(glEnable(GL_DEPTH_TEST));
	
	//----Sky box
	Cube* textureCubes[1];
	textureCubes[0] = new TextureCube(0, 0, 0,"res/sky.bmp");
	textureCubes[0]->setSize(200, 200, 200);
	
	//----Crate cubes
	Cube* cubes[1];
	cubes[0] = new TextureCube(0,-0.5f,0, "res/test2.bmp");
	cubes[0]->setSize(10, 1, 10);

	//Model mario("res/Objs/Mario", "FitMario00.obj", 1);
	Model nano("res/Objs/nanosuit", "nanosuit.obj", 1);
	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	nano.getShader()->setUniformMat4f("Projection", projection);
	nano.getShader()->setUniformMat4f("World", glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)));
	/*mario.getShader()->setUniformMat4f("Projection", projection);
	mario.getShader()->setUniformMat4f("World", glm::scale(glm::vec3(0.1f, 0.1f, 0.1f)));*/



	//----Camera
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(0,0,-1));

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
		/*mario.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		mario.draw();*/
		nano.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		nano.draw();

		render.flush();
		window.update();
	}

	for (auto& cube : cubes)
		delete cube;

	for (auto& cube : textureCubes)
		delete cube;

	return 0;
};

