#include "window.h"
#include "Model.h"
#include "Camera.h"
#include "LightBill.h"

void renderQuad()
{
	static unsigned int quadVAO = 0;
	static unsigned int quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

main
{
	Window window("Deferred", 1280,720);
	window.hideAndGrabMouseMode();
	
	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	Model nano("res/Objs/nanosuit", "nanosuit.obj", 1);
	
	LightBill light(5, 8, 5);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	

	GLuint gBuffer;
	GLCall(glGenFramebuffers(1, &gBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, gBuffer));
	GLuint gPosition, gNormal, gColorSpec;

	// Position color buffer
	GLCall(glGenTextures(1, &gPosition));
	GLCall(glBindTexture(GL_TEXTURE_2D, gPosition));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0));

	// Normal COlor buffer
	GLCall(glGenTextures(1, &gNormal));
	GLCall(glBindTexture(GL_TEXTURE_2D, gNormal));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 720, 0, GL_RGB, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0));

	// Color + specular buffer
	GLCall(glGenTextures(1, &gColorSpec));
	GLCall(glBindTexture(GL_TEXTURE_2D, gColorSpec));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0));

	GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	GLCall(glDrawBuffers(3, attachments));

	GLuint rboDepth;
	GLCall(glGenRenderbuffers(1, &rboDepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Det sket sig!" << std::endl;
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/objVert.glsl" },
		{GL_FRAGMENT_SHADER, "shaders/objGeoPassFrag.glsl"},
		{GL_NONE, NULL}
	};

	ShaderInfo shaders2[] =
	{
		{GL_VERTEX_SHADER, "shaders/objLightPassVert.glsl"},
		{GL_FRAGMENT_SHADER, "shaders/objLightPassFrag.glsl"},
		{GL_NONE,NULL}
	};

	Shader firstPass(shaders);
	Shader lightPass(shaders2);
	lightPass.setUniform1i("gPosition", 0);
	lightPass.setUniform1i("gNormal", 1);
	lightPass.setUniform1i("gColorSpec", 2);
	Camera camera(window.getWidth(), window.getHeight(), glm::vec3(-0.5, -0.2, -0.9), glm::vec3(12,7,18));

	while (window.closed())
	{
		window.clear();
		double x, y;
		static float power = 1.0f;
		window.getMousePosition(x, y);
		camera.update(x, y);

		//---Adjust lightpower
		if (GetAsyncKeyState(int('M')))
			power += 0.5f;
		if (GetAsyncKeyState(int('N')))
		{
			std::cout << "Pos:x:" << camera.getPos().x << " y:" << camera.getPos().y << " z:" << camera.getPos().z << "\n";
			std::cout << "Dir:x:" << camera.getTarget().x << " y:" << camera.getTarget().y << " z:" << camera.getTarget().z << "\n";
		}



		// Geo pass
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		firstPass.bind();
		firstPass.setUniformMat4f("Projection", projection);
		firstPass.setUniformMat4f("View", camera.getViewMatrix());
		firstPass.setUniform3f("Light_pos", light.getPosition());
		firstPass.setUniform3f("Camera_Pos", camera.getPos());
		firstPass.setUniform1f("lightPower", power);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				firstPass.setUniformMat4f("World", glm::translate(glm::vec3(5*i,0,5*j)) * glm::scale(glm::rotate(0.0f, glm::vec3(0, 1, 0)), glm::vec3(0.4, 0.4, 0.4)));
				nano.draw(&firstPass);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Light pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightPass.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColorSpec);

		lightPass.setUniform3f("Light_pos", light.getPosition());
		lightPass.setUniform3f("Camera_pos", camera.getPos());

		renderQuad();

		light.draw(camera, projection);

		window.update();
	}

	return 0;

}