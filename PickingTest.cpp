#include "window.h"
#include "Camera.h"
#include "Skybox.h"
#include "Model.h"
#include "LightBill.h"
#include "PickingTexture.h"

main
{
	Window window("PickingTest", 1280, 720);
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(-0.5, -0.2, -0.9), glm::vec3(12, 7, 18));
	Skybox box("res/skyboxes/Skybox2/skybox", GL_TEXTURE7);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	Model nanosuit("res/Objs/nanosuit", "nanosuit.obj", 1);
	glm::mat4 nanoPos = glm::scale(glm::translate(glm::vec3(0, 1, 0)), glm::vec3(0.2, 0.2, 0.2));

	nanosuit.getShader()->setUniformMat4f("Projection", projection);
	nanosuit.getShader()->setUniformMat4f("World", nanoPos);

	Model floor("res/Objs/woodenfloor", "floor.obj", 1);
	floor.getShader()->setUniformMat4f("Projection", projection);
	floor.getShader()->setUniformMat4f("World", glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(5, 5, 5)));

	glm::mat4 positions[4];
	positions[0] = glm::scale(glm::translate(glm::vec3(0, 3, 5)), glm::vec3(5, 3, 3)) * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	positions[1] = glm::scale(glm::translate(glm::vec3(0, 3, -5)), glm::vec3(5, 3, 3));
	positions[2] = glm::translate(glm::vec3(-5, 3, 0)) *glm::rotate((3.141592f / 2.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 3, 3));
	positions[3] = glm::translate(glm::vec3(5, 3, 0)) *glm::rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(5, 3, 3));

	Model* walls[4];
	walls[0] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[0]->getShader()->setUniformMat4f("Projection", projection);
	walls[0]->getShader()->setUniformMat4f("World", positions[0]);

	walls[1] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[1]->getShader()->setUniformMat4f("Projection", projection);
	walls[1]->getShader()->setUniformMat4f("World", positions[1]);

	walls[2] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[2]->getShader()->setUniformMat4f("Projection", projection);
	walls[2]->getShader()->setUniformMat4f("World", positions[2]);

	walls[3] = new Model("res/Objs/brickwall", "wall.obj", 1);
	walls[3]->getShader()->setUniformMat4f("Projection", projection);
	walls[3]->getShader()->setUniformMat4f("World", positions[3]);

	LightBill light(0, 8, 0);
	
	PickingTexture pt;
	pt.init(window.getWidth(), window.getHeight());

	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/picking.vs"},
		{GL_FRAGMENT_SHADER, "shaders/picking.fs"},
		{GL_NONE, NULL}
	};

	ShaderInfo shadersSingleColor[] =
	{
		{GL_VERTEX_SHADER, "shaders/stencil_single_color.vs"},
		{ GL_FRAGMENT_SHADER, "shaders/stencil_single_color.fs" },
		{GL_NONE, 0}
	};


	Shader pickingShader(shaders);
	Shader simpleShader(shadersSingleColor);

	int drawIndex = 0;
	bool checkMouse = false;
	while (window.closed())
	{
		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		
		if (GetAsyncKeyState(VK_SPACE))
		{
			window.restoreMouse();
		}
		else
		{
			camera.update(x, y);
			window.hideAndGrabMouseMode();
		}

		pt.enableWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < 4; i++)
		{

			//Picking 
			pickingShader.setUniformMat4f("P", projection);
			pickingShader.setUniformMat4f("V", camera.getViewMatrix());
			pickingShader.setUniformMat4f("M", positions[i]);
			pickingShader.setUniform1ui("drawIndex", drawIndex);
			pickingShader.setUniform1ui("objectIndex", drawIndex++);
			walls[i]->draw(&pickingShader);

		}

		pickingShader.setUniformMat4f("P", projection);
		pickingShader.setUniformMat4f("V", camera.getViewMatrix());
		pickingShader.setUniformMat4f("M", nanoPos);
		pickingShader.setUniform1ui("drawIndex", drawIndex);
		pickingShader.setUniform1ui("objectIndex", drawIndex++);
		nanosuit.draw(&pickingShader);

		pt.disableWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int index = 0;
		if (GetAsyncKeyState(VK_SPACE))
		{
			PickingTexture::PixelInfo pixel = pt.readPixel((x), (window.getHeight() - y - 1));
			if (pixel.objectID != 0)
			{
				index = pixel.objectID;
			}
		}
		
	
		glStencilMask(0x00);
		box.render(camera);
		for (int i = 0; i < 4; i++)
		{
			
			walls[i]->getShader()->setUniformMat4f("View", camera.getViewMatrix());
			walls[i]->getShader()->setUniform3f("Light_pos", light.getPosition());
			walls[i]->getShader()->setUniform3f("Camera_Pos", camera.getPos());
			walls[i]->getShader()->setUniform1f("lightPower", 40.0f);
			walls[i]->getShader()->setUniformMat4f("World", positions[i]);
			walls[i]->draw();

		}

		nanosuit.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		nanosuit.getShader()->setUniform3f("Light_pos", light.getPosition());
		nanosuit.getShader()->setUniform3f("Camera_Pos", camera.getPos());
		nanosuit.getShader()->setUniform1f("lightPower", 40.0f);
		nanosuit.getShader()->setUniformMat4f("World", nanoPos);
		nanosuit.draw();

		floor.getShader()->setUniformMat4f("View", camera.getViewMatrix());
		floor.getShader()->setUniform3f("Light_pos", light.getPosition());
		floor.getShader()->setUniform3f("Camera_Pos", camera.getPos());
		floor.getShader()->setUniform1f("lightPower", 40.0f);
		floor.draw();

		if (index != 0)
		{

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			float scale = 1.05;

			Model* currentModel = &nanosuit;

			glm::mat4 model = nanoPos;

			if (index != 5)
			{
				model = positions[index - 1];
				currentModel = walls[index - 1];
			}

			currentModel->draw();

			glDisable(GL_DEPTH_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			simpleShader.setUniformMat4f("view", camera.getViewMatrix());
			simpleShader.setUniformMat4f("projection", projection);
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			simpleShader.setUniformMat4f("model", model);

			currentModel->draw(&simpleShader);

			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);


		}

		drawIndex = 1;
		

		//// mat4 Projection
		glm::mat4 ViewMatrix = camera.getViewMatrix();

		glm::vec3 cameraRight_worldspace = glm::vec3(ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glm::vec3 cameraUp_worldspace = glm::vec3(ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		//
		light.getShader()->setUniform3f("CameraRightWorld", cameraRight_worldspace);
		light.getShader()->setUniform3f("CameraUpWorld", cameraUp_worldspace);

		glm::mat4 ViewProjMatrix = projection * ViewMatrix;
		light.getShader()->setUniformMat4f("ViewProj", ViewProjMatrix);

		light.draw();

		window.update();
	}
}