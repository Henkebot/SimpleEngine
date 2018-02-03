#include "Skybox.h"
#include <string>
#include <iostream>

Skybox::Skybox(const char * skybox, GLenum texture)
{
	_initBox();
	_initTexture(skybox, texture);
	_initShaders();

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);

	m_pShader->setUniformMat4f("Projection", projection);
}

Skybox::~Skybox()
{
	delete m_pIndexBuffer;
	delete m_pShader;
}

void Skybox::render(const Camera & camera)
{
	glDisable(GL_DEPTH_TEST);
	
	m_pShader->bind();
	m_Vao.bind();
	m_pIndexBuffer->bind();
	m_pShader->setUniformMat4f("View", camera.getViewMatrix());
	m_pShader->setUniformMat4f("World", glm::translate(camera.getPos()));

	glDrawElements(GL_TRIANGLES, m_pIndexBuffer->getIndices(), GL_UNSIGNED_SHORT, 0);

	glEnable(GL_DEPTH_TEST);
	
}

void Skybox::_initBox()
{
	static const float skyboxData[] = {

		-10.0f, -10.0f, -10.0f, 1.0,
		10.0f, -10.0f, -10.0f, 1.0,
		10.0f, 10.0f, -10.0f, 1.0,
		-10.0f, 10.0f, -10.0f, 1.0,

		-10.0f, -10.0f, 10.0f, 1.0,
		10.0f, -10.0f, 10.0f, 1.0,
		10.0f, 10.0f, 10.0f, 1.0,
		-10.0f, 10.0f, 10.0f, 1.0,
	};

	static const unsigned short skyboxIndices[] = {
		// Neg Z face
		0, 1, 2,
		0, 2, 3,

		// Neg X face
		4, 0, 3,
		4, 3, 7,

		// Pos Z face
		4, 7, 5,
		5, 7, 6,

		// Pos X face
		5, 6, 2,
		1, 5, 2,

		// Pos y face
		3, 6, 7,
		3, 2, 6,

		// Neg y face
		0, 4, 5,
		0, 5, 1
	};

	m_Vao.addBuffer(new Buffer(skyboxData, sizeof(skyboxData), 4), 0);
	m_pIndexBuffer = new IndexBuffer(skyboxIndices, sizeof(skyboxIndices));

}

void Skybox::_initTexture(const char* skyboxPath, GLenum texture)
{
	
	auto initImage = [&](const char* src, int type) -> unsigned char* {
		std::cout << "SkyBox: Reading BMP " << src;
		unsigned char header[54];	// Each BMP file begins by a 54-bytes header
		unsigned int dataPos;		// Position in the file where the actual data begins
		unsigned int imageSize;		// = width * height * 3
		unsigned int width, height;
		unsigned char* data;	// actuall RGB data

		FILE* file;
		fopen_s(&file, src, "rb");
		if (!file)
		{
			std::cout << "Error: Couldnt open (" << src << ")\n";
			return nullptr;
		}

		if (fread(header, 1, 54, file) != 54)
		{
			std::cout << "Error: Not a correct BMP file (" << src << ")\n";
			return nullptr;
		}

		if (header[0] != 'B' || header[1] != 'M')
		{
			std::cout << "Error: Not a correct BMP file (" << src << ")\n";
		}

		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		// Sometimes the BMP files are misformatted, time to guess information!
		if (imageSize == 0) imageSize = width * height * 3;
		if (dataPos == 0) dataPos = 54;

		data = new unsigned char[imageSize];
		fread(data, 1, imageSize, file);
		fclose(file);

		if (data != nullptr)
		{
			glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed load BMP(" << src << ")." << std::endl;
		}
		std::cout << "...Done!" << std::endl;
		delete data;
	};
	
	std::string path = skyboxPath;
	//glActiveTexture(texture);
	glEnable(GL_TEXTURE_CUBE_MAP);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	initImage((path + "PosX.bmp").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	initImage((path + "NegX.bmp").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);

	initImage((path + "PosY.bmp").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	initImage((path + "NegY.bmp").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);

	initImage((path + "PosZ.bmp").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	initImage((path + "NegZ.bmp").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	
	//glActiveTexture(0);
}

void Skybox::_initShaders()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/skyboxVertex.glsl"},
		{ GL_FRAGMENT_SHADER, "shaders/skyboxFrag.glsl" },
		{ GL_NONE, NULL }
	};

	m_pShader = new Shader(shaders);
}
