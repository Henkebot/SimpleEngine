#include <crtdbg.h>

#include "window.h"
#include "Render2D.h"

#include "RenderObject2D.h"
#include "RenderObject3D.h"

#include "Camera.h"

#define WIDTH 1280.0f
#define HEIGHT 720.0f

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window window("Test", WIDTH, HEIGHT);
	Render2D render;

	Camera camera(WIDTH, HEIGHT,glm::vec3(0,0,3), glm::vec3(0, 0, 1));

	//RenderObject2D test({glm::vec2(-1.0f,0), glm::vec2(0,0), glm::vec2(0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)});
	//RenderObject* square = new RenderObject2D(glm::vec2(-1.0f, -1.0f), glm::vec2(2.0f,2.0f));
	RenderObject* cube = new RenderObject3D(glm::vec3(0,0,-2.0f));
	RenderObject* light = new RenderObject3D(glm::vec3(3, 5, -5.0f));

	glEnable(GL_DEPTH_TEST);
	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);

		camera.update(x, y);

		static const GLfloat aspect = WIDTH / HEIGHT;
		
		static glm::mat4 persp = glm::perspective(70.0f, aspect, 1.0f, 200.0f);
		glm::mat4 view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getTarget(), camera.getUp());
		
		
		static float angle = 0.0f;
		angle += 0.01f;
		light->setUniformMat4("Projection", persp);
		light->setUniformMat4("View", view);
		light->translate(glm::vec3(0, 0, -angle));
		light->updateUniforms();
		render.submit(light);

		cube->setUniformMat4("Projection", persp);
		cube->setUniformMat4("View", view);
		cube->setUniform3f("light_pos", light->getPos3fv());
		//cube->translate(glm::vec3(0, 0, -angle));
		cube->rotate(angle, glm::vec3(0, 1, 0));
		cube->updateUniforms();
		render.submit(cube);


		render.flush();
		window.update();
	}
	delete cube;
	return 0;
}
