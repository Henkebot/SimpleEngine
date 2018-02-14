#include "window.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"
#include "LightBill.h"
#include "ShadowMap.h"




main
{
	Window window("ShadowMappingTest", 1280, 720);
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(-0.5, -0.2, -0.9), glm::vec3(12, 7, 18));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	Model floor("res/Objs/woodenfloor", "floor.obj", 1);
	floor.getShader()->setUniformMat4f("Projection", projection);
	floor.getShader()->setUniformMat4f("World", glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(20, 20, 20)));
	
	Model nanosuit("res/Objs/nanosuit", "nanosuit.obj", 1);
	glm::mat4 nanoPos = glm::scale(glm::translate(glm::vec3(0, 1, 0)), glm::vec3(0.2, 0.2, 0.2));
	nanosuit.getShader()->setUniformMat4f("Projection", projection);
	nanosuit.getShader()->setUniformMat4f("World", nanoPos);
	glm::mat4 positions[4];

	positions[0] = glm::translate(glm::vec3(0,2,5))   * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	positions[1] = glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)) *  glm::translate(glm::vec3(0, 1, 5));
	positions[2] = glm::rotate(glm::radians(46.0f), glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(2, 5, -5));
	positions[3] = glm::translate(glm::vec3(1, 3, 5)) * glm::rotate(glm::radians(75.0f), glm::vec3(0, 1, 0));

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


	ShaderInfo shadowShaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/shadow_mapping_box.vs"},
		{GL_FRAGMENT_SHADER, "shaders/shadow_mapping_box.fs"},
		{GL_NONE, NULL}
	};

	ShaderInfo lightShadowShaders[] = 
	{
		{GL_VERTEX_SHADER, "depthShader.vert"},
		{GL_FRAGMENT_SHADER, "depthShader.frag"},
		{GL_NONE, NULL}
	};

	Shader shadowShader(shadowShaders);
	shadowShader.setUniform1i("shadowMap", 8);


	Shader lightShadowShader(lightShadowShaders);
	lightShadowShader.setUniform1i("depthMap", 0);

	ShadowMap shadowMap(1024, 1024);

	window.hideAndGrabMouseMode();
	while (window.closed())
	{
		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		camera.update(x, y);

		if (GetAsyncKeyState(int('C')))
			light.move(glm::vec3(0, 0, 1));
		else if (GetAsyncKeyState(int('V')))
			light.move(glm::vec3(0, 0, -1));

		// 1. Render depth of scene to texture( from light's perspective
		lightShadowShader.setUniformMat4f("lightSpaceMatrix", shadowMap.getlightSpaceMatrix(light.getPosition()));
		shadowMap.bindFBO();
		glCullFace(GL_FRONT);
		for (int i = 0; i < 4; i++)
		{
			lightShadowShader.setUniformMat4f("World", positions[i]);
			walls[i]->draw(&lightShadowShader);

		}
		lightShadowShader.setUniformMat4f("World", nanoPos);
		nanosuit.draw(&lightShadowShader);
		/*lightShadowShader.setUniformMat4f("World", glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(20, 20, 20)));
		floor.draw(&lightShadowShader);*/
		glFrontFace(GL_CW);
		shadowMap.unbindFBO();


		// 1.1 Reset viewport
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 4; i++)
		{
			shadowShader.setUniformMat4f("Projection", projection);
			shadowShader.setUniformMat4f("View", camera.getViewMatrix());
			shadowShader.setUniformMat4f("World", positions[i]);
			shadowShader.setUniform3f("viewPos", camera.getPos());
			shadowShader.setUniform3f("lightPos", light.getPosition());
			shadowShader.setUniformMat4f("lightSpaceMatrix", shadowMap.getlightSpaceMatrix(light.getPosition()));
			glActiveTexture(GL_TEXTURE8);
			shadowMap.bindDepthMap();
			walls[i]->draw(&shadowShader);

		}
		shadowShader.setUniformMat4f("World", nanoPos);
		nanosuit.draw(&shadowShader);

		shadowShader.setUniformMat4f("World", glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(20, 20, 20)));
		floor.draw(&shadowShader);

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