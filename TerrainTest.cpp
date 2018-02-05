#include "terrain\Terrain.h"
#include "window.h"

main
{
	Window window("TerrainTest", 1280, 720);
	window.hideAndGrabMouseMode();
	
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(0, 0, -1), glm::vec3(0,400,10));

	Terrain terrain("res/terrains/terrain2/terrainSettings.txt", &camera);

	while (window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		camera.update(x, y);
		terrain.update();
		terrain.render();

		window.update();
	}

}
