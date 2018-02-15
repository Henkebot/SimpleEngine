#include "terrain\Terrain.h"
#include "Skybox.h"
#include "window.h"
#include "Model.h"
#include "LightBill.h"
main
{
	Window window("TerrainTest", 1280, 720);
	window.hideAndGrabMouseMode();

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 2000000.0f);
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(0, 0, -1), glm::vec3(0,0,10));
	//----Skybox
	Skybox box("res/skyboxes/Skybox2/skybox", GL_TEXTURE7);
	LightBill light(0, 300, 0);

	
	Terrain terrain("res/terrains/terrain2/terrainSettings.txt", &camera);
	while (window.closed())
	{
		window.clear();
		
		if(GetAsyncKeyState(VK_UP))
		{
			std::cout << Terrain::VMB_TERRAIN_REC_CUTOFF++ << std::endl;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{
			Terrain::VMB_TERRAIN_REC_CUTOFF = max(0, --Terrain::VMB_TERRAIN_REC_CUTOFF);
			std::cout << Terrain::VMB_TERRAIN_REC_CUTOFF << std::endl;
		}
		box.render(camera);
		double x, y;
		window.getMousePosition(x, y);

		camera.update(x, y);
		terrain.getShader()->setUniform3f("Light_Pos", light.getPosition());
		terrain.getShader()->setUniform3f("Camera_Pos", camera.getPos());

		light.draw(camera, projection);
		terrain.update();
		terrain.render();

		window.update();
	}

}
