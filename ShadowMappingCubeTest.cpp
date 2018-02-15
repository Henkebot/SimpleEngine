#include "window.h"
#include "Camera.h"
#include "Model.h"
#include <gtc/matrix_transform.hpp>
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
	glm::mat4 floorPos = glm::rotate(3.141592f / 2.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(200, 200, 200));


	glm::mat4 positions[4];
	positions[0] = glm::translate(glm::vec3(0,2,5))   * glm::rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	positions[1] = glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0)) *  glm::translate(glm::vec3(0, 1, 5));
	positions[2] = glm::rotate(glm::radians(46.0f), glm::vec3(0, 1, 0)) * glm::translate(glm::vec3(2, 5, -5));
	positions[3] = glm::translate(glm::vec3(1, 3, 5)) * glm::rotate(glm::radians(75.0f), glm::vec3(0, 1, 0));

	Model* wall = new Model("res/Objs/brickwall", "wall.obj", 1);
	
	LightBill light(0, 8, 0);


	ShaderInfo shadowShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/point_shadow_mapping.vs" },
		{ GL_FRAGMENT_SHADER, "shaders/point_shadow_mapping.fs" },
		{ GL_NONE, NULL }
	};

	ShaderInfo lightShadowShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/point_shadows_depth.vs" },
		{ GL_GEOMETRY_SHADER, "shaders/point_shadows_depth.gs" },
		{ GL_FRAGMENT_SHADER, "shaders/point_shadows_depth.fs" },
		{ GL_NONE, NULL }
	};

	Shader shader(shadowShaders);
	
	Skybox box("res/skyboxes/Skybox2/skybox", GL_TEXTURE0);
	Shader simpleDepthShader(lightShadowShaders);
	

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader.setUniform1i("depthMap", 2); // KOLLA DETTA!
	window.hideAndGrabMouseMode();
	while (window.closed())
	{
		window.clear();
		double x, y;
		window.getMousePosition(x, y);
		camera.update(x, y);
		
		static bool shift = false;
		if (GetAsyncKeyState(VK_SHIFT))
			shift = true;
		else
			shift = false;
		if (GetAsyncKeyState(int('C')))
		{
			
			light.move(glm::vec3(0, 0, shift ? 1 : -1));
		}
		else if (GetAsyncKeyState(int('V')))
		{

			light.move(glm::vec3(shift ? 1 : -1, 0,0));
		}
		else if (GetAsyncKeyState(int('X')))
		{

			light.move(glm::vec3(0, shift ? 1 : -1, 0));
		}

		// 0. Create depth cubemap transformation matrices
		float near_plane = 1.0f;
		float far_plane = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		// Projection * View, but in shadows
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(light.getPosition(), light.getPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		
		// 1. Render depth of scene to texture( from light's perspective
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		for (unsigned int i = 0; i < 6; ++i)
		{
			std::string index = "shadowMatrices[";
			index += std::to_string(i);
			index += "]";
			simpleDepthShader.setUniformMat4f(index.c_str(), shadowTransforms[i]);
		}
		simpleDepthShader.setUniform1f("far_plane", far_plane);
		simpleDepthShader.setUniform3f("lightPos", light.getPosition());
		for (int i = 0; i < 4; i++)
		{
			simpleDepthShader.setUniformMat4f("model", positions[i]);
			wall->draw(&simpleDepthShader);
		}
		simpleDepthShader.setUniformMat4f("model", floorPos);
		floor.draw(&simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		box.render(camera);
		glm::mat4 view = camera.getViewMatrix();
		shader.bind();
		shader.setUniformMat4f("Projection", projection);
		shader.setUniformMat4f("View", view);
		// set lighting uniforms
		shader.setUniform3f("Light_Pos", light.getPosition());
		shader.setUniform3f("Camera_pos", camera.getPos());
		shader.setUniform1f("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (int i = 0; i < 4; i++)
		{
			shader.setUniformMat4f("World", positions[i]);
			wall->draw(&shader);
		}
		shader.setUniformMat4f("World", floorPos);
		floor.draw(&shader);
		
	
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


