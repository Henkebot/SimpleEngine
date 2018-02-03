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

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);
	
	//----Crate Floor
	Model floor("res/Objs/woodenfloor", "floor.obj", 1);
	floor.getShader()->setUniformMat4f("Projection", projection);
	floor.getShader()->setUniformMat4f("World", glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5,5,5)));
	//----Models
	Model nano("res/Objs/nanosuit", "nanosuit.obj", 1);

	
	nano.getShader()->setUniformMat4f("Projection", projection);
	nano.getShader()->setUniformMat4f("World", glm::scale(glm::translate(glm::vec3(0,1,0)),glm::vec3(1, 1, 1)));

	Model* walls[4];
	walls[0] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[0]->getShader()->setUniformMat4f("Projection", projection);
	walls[0]->getShader()->setUniformMat4f("World",  glm::scale(glm::translate(glm::vec3(0, 3, 5)), glm::vec3(5,3,3)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0)));

	walls[1] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[1]->getShader()->setUniformMat4f("Projection", projection);
	walls[1]->getShader()->setUniformMat4f("World", glm::scale(glm::translate(glm::vec3(0, 3, -5)), glm::vec3(5, 3, 3)));

	walls[2] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[2]->getShader()->setUniformMat4f("Projection", projection);
	walls[2]->getShader()->setUniformMat4f("World", glm::translate(glm::vec3(-5, 3, 0)) *glm::rotate((3.141592f / 2.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 3, 3)));

	walls[3] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[3]->getShader()->setUniformMat4f("Projection", projection);
	walls[3]->getShader()->setUniformMat4f("World", glm::translate(glm::vec3(5, 3, 0)) *glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 3, 3)));
	
	//----Light
	LightBill light(0, 8, 0);

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
		static float power = 0.0f;
	
		window.getMousePosition(x, y);

		camera.update(x,y);

		//---Skybox
		box.render(camera);

		if(GetAsyncKeyState(int('M')))
			power += 0.5f;
		if (GetAsyncKeyState(int('N')))
			power -= 0.5f;


		nano.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		nano.getShader()->setUniform3f("Light_pos", light.getPosition());
		nano.getShader()->setUniform3f("Camera_Pos", camera.getPos());
		nano.getShader()->setUniformMat4f("World", glm::scale(glm::rotate(0.0f,glm::vec3(0, 1, 0)), glm::vec3(0.4, 0.4, 0.4)));
		nano.getShader()->setUniform1f("lightPower", power);
		nano.draw();

		for (auto& wall : walls)
		{
			wall->getShader()->setUniformMat4f("View", camera.getViewMatrix());
			wall->getShader()->setUniform3f("Light_pos", light.getPosition());
			wall->getShader()->setUniform3f("Camera_Pos", camera.getPos());
			wall->getShader()->setUniform1f("lightPower", power);
			wall->draw();
		}

		floor.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		floor.getShader()->setUniform3f("Light_pos", light.getPosition());
		floor.getShader()->setUniform3f("Camera_Pos", camera.getPos());
		floor.getShader()->setUniform1f("lightPower", power);
		floor.draw();

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


	return 0;
};

