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
	RenderObject* light = new RenderObject3D(glm::vec3(0.0f, 0.0f,-2.0f), glm::vec3(0.5,0.5f,0.5f));

	glEnable(GL_DEPTH_TEST);
	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);

		camera.update(x, y);

		static const GLfloat aspect = WIDTH / HEIGHT;
		static GLfloat viewDistance = 1.0f;
		static glm::mat4 ortho = glm::ortho(-aspect * viewDistance, aspect* viewDistance, -viewDistance, viewDistance, -1000.0f, 1000.0f);
		static glm::mat4 persp = glm::perspective(70.0f, aspect, 0.01f, 200.0f);
		glm::mat4 view = glm::lookAt(camera.getPos(), camera.getPos() + camera.getTarget(), camera.getUp());
		//std::cout << "Light pos: " << ((camera.getPos() + camera.getTarget()).x) << "," << ((camera.getPos() + camera.getTarget()).y) << "," << ((camera.getPos() + camera.getTarget()).z) << std::endl;
		
		
		static float angle = 0.0f;
		static float moveZ = 0.0f;
		
		light->setUniformMat4("Projection", persp);
		light->setUniformMat4("View", view);
		light->setUniform3f("light_pos", light->getPos3fv());
		light->setUniform3f("ViewPos", camera.getPos());
		//light->rotate(angle, glm::vec3(0, 1, 0));
		if (GetAsyncKeyState(int('V')))
		{
			angle += 0.01f;;
			moveZ = -2.0f;
			light->translate(glm::vec3(0, 0, moveZ));
			light->rotate(angle, glm::vec3(0, 1, 0));
			light->rotate(angle, glm::vec3(1, 1, 0));
		}
		else if (GetAsyncKeyState(int('C')))
		{
			moveZ -= 0.5f;
			light->translate(glm::vec3(0, 0, moveZ));
		}

		light->updateUniforms();
		render.submit(light);

		cube->setUniformMat4("Projection", persp);
		cube->setUniformMat4("View", view);
		cube->setUniform3f("light_pos", light->getPos3fv());
		cube->setUniform3f("ViewPos", camera.getPos());
		//std::cout << "Light pos: " << (light->getPos3fv().x) << "," << (light->getPos3fv().y) << "," << (light->getPos3fv().z) << std::endl;
		//cube->translate(glm::vec3(0, 0, -angle));
		//cube->rotate(angle, glm::vec3(0, 1, 0));
		cube->updateUniforms();
		render.submit(cube);


		render.flush();
		window.update();
	}
	delete cube;
	return 0;
}
