#include "window.h"

#include "Cube.h"
#include "TextureCube.h"
#include "Render2D.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"
#include "LightBill.h"

main
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//----Create Window
	Window window("Test",1280,720);
	window.hideAndGrabMouseMode();
	
	//----OpenGL Settings
	GLCall(glEnable(GL_DEPTH_TEST));
	
	//----Crate cubes
	Cube* cubes[1];
	cubes[0] = new TextureCube(0,-0.5f,0, "res/test2.bmp");
	cubes[0]->setSize(10, 1, 10);
	
	//----Models
	Model nano("res/Objs/nanosuit", "nanosuit.obj", 1);
	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	nano.getShader()->setUniformMat4f("Projection", projection);
	nano.getShader()->setUniformMat4f("World", glm::scale(glm::translate(glm::vec3(0,1,0)),glm::vec3(1, 1, 1)));

	//----Light
	LightBill light(0, 15, 10);

	//----Skybox
	Skybox box("res/skyboxes/Skybox2/skybox", GL_TEXTURE0);

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
		box.render(camera);

		for (auto& cube : cubes)
		{
			cube->setCamMatrix(camera);
			render.submit(cube);
		}

		nano.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		nano.getShader()->setUniform3f("Light_pos", light.getPosition());
		nano.getShader()->setUniform3f("Camera_Pos", camera.getPos());
		nano.draw();



		//----Light

		// mat4 Projection
		glm::mat4 ViewMatrix = camera.getViewMatrix();

		glm::vec3 cameraRight_worldspace = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glm::vec3 cameraUp_worldspace = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		light.getShader()->setUniform3f("CameraRightWorld", cameraRight_worldspace);
		light.getShader()->setUniform3f("CameraUpWorld", cameraUp_worldspace);

		glm::mat4 ViewProjMatrix = projection * ViewMatrix;
		light.getShader()->setUniformMat4f("ViewProj", ViewProjMatrix);

		light.draw();
	

		render.flush();
		window.update();
	}

	for (auto& cube : cubes)
		delete cube;

	return 0;
};

