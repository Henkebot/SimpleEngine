#include "terrain\Terrain.h"
#include "Skybox.h"
#include "window.h"
#include "Model.h"
#include "LightBill.h"

main
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Window window("Gustav och Oskar och Johanna", 1280, 720);
	window.hideAndGrabMouseMode();

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 2000000.0f);
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(0, 0, -1), glm::vec3(0,0,10));
	//----Skybox
	Skybox box("res/skyboxes/Skybox2/skybox", GL_TEXTURE7);
	LightBill light(0, 300, 0);
	LightBill light2(512, 2, -512);

	
	Terrain terrain("res/terrains/terrain2/terrainSettings.txt", &camera);

	Camera* currentCamera = &camera;
	while (window.closed())
	{
		window.clear();
		
		box.render(*currentCamera);
		double x, y;
		window.getMousePosition(x, y);

		currentCamera->update(x, y);
		static float lol = 0.1f;
		//lol += 0.01f;
		//light.move(glm::vec3(std::sin(lol) * 300, 300, std::cos(lol) * 300));
		terrain.getShader()->setUniform3f("Light_Pos", light.getPosition());

		light.draw(*currentCamera, projection);
		light2.draw(*currentCamera, projection);
		if(!GetAsyncKeyState(int('C')))
			terrain.update();
		terrain.render();

		window.update();
	}

}
