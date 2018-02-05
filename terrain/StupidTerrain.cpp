#include "StupidTerrain.h"
#include <iostream>
#include "../lodepng.h"
#include "../Camera.h"
#include "../TGALoader.h"

int StupidTerrain::_loadScene(const char * sceneFile)
{
	_initScene();
	FILE* file;
	fopen_s(&file, sceneFile, "r");
	if (file == nullptr)
	{
		std::cout << "Terrain: Failed to open " << sceneFile << std::endl;
	}

	while (1)
	{
		char line[128];
		int result = fscanf_s(file, "%s", line, sizeof(line));
		if (result == EOF)
			break;

		if (!strcmp(line, "TerrainWidth"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.terrainWidth = value;
		}
		else if (!strcmp(line, "TerrainHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.terrainHeight = value;
		}

		else if (!strcmp(line, "HeightMap"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.heightMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "NormalMap"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.normalMap, 50 * sizeof(char), location);
		}

		else if (!strcmp(line, "TexBase"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.texBase.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "TexBaseNormal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.texBase.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "TexBaseMipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.texBase.mipLevels = value;
		}
		else if (!strcmp(line, "TexBaseSoftHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.texBase.softHeight = value;
		}
		else if (!strcmp(line, "TexBaseHardHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.texBase.hardHeight = value;
		}

		else if (!strcmp(line, "Tex0"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex0.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex0Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex0.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex0MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex0.mipLevels = value;
		}
		else if (!strcmp(line, "Tex0SoftHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex0.softHeight = value;
		}
		else if (!strcmp(line, "Tex0HardHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex0.hardHeight = value;
		}

		else if (!strcmp(line, "Tex1"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex1.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex1Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex1.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex1MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex1.mipLevels = value;
		}
		else if (!strcmp(line, "Tex1SoftHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex1.softHeight = value;
		}
		else if (!strcmp(line, "Tex1HardHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex1.hardHeight = value;
		}

		else if (!strcmp(line, "Tex2"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex2.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex2Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex2.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex2MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex2.mipLevels = value;
		}
		else if (!strcmp(line, "Tex2SoftHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex2.softHeight = value;
		}
		else if (!strcmp(line, "Tex2HardHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex2.hardHeight = value;
		}

		else if (!strcmp(line, "Tex3"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex3.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex3Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(m_Scene.tex3.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex3MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex3.mipLevels = value;
		}
		else if (!strcmp(line, "Tex3SoftHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex3.softHeight = value;
		}
		else if (!strcmp(line, "Tex3HardHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			m_Scene.tex3.hardHeight = value;
		}
		else
			std::cout << "Error reading " << sceneFile << std::endl;
	}
	fclose(file);

	if (m_Scene.terrainHeight == 0.0)
		m_Scene.terrainHeight = 1.0;
	if (m_Scene.terrainWidth == 0.0)
		m_Scene.terrainWidth = 1.0;
	return 1;
}

void StupidTerrain::_initGraphics()
{
	float quadDataVert[] = {
		// Vert 1
		-20.0f, 0.0f, -20.0f, 1.0,	// Position
		// Vert 2
		20.0f, 0.0f, -20.0f, 1.0,	// Position
		// Vert 3
		20.0f, 0.0f, 20.0f, 1.0,	// Position
		// Vert 4
		-20.0f, 0.0f, 20.0f, 1.0,	// Position
	};
	float quadDataColors[] =
	{
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color

	};

	float quadDataNormals[] =
	{
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
	};
	
	float quadDataUVs[] =
	{
		0.0f, 2.0f,					// Tex coord (u,v)
		2.0f, 2.0f,					// Tex coord (u,v)
		2.0f, 0.0f,					// Tex coord (u,v)
		0.0f, 0.0f,					// Tex coord (u,v)

	};
	GLuint quadIndicies[] = { 0, 3, 2, 0, 2, 1 };
	GLushort quadPatchInd[] = { 0, 1, 2, 3 };


	m_Vao.addBuffer(new Buffer(quadDataVert, sizeof(quadDataVert), 4), 0);
	m_Vao.addBuffer(new Buffer(quadDataUVs, sizeof(quadDataUVs), 2), 1);

	m_IndexBuffer = new IndexBuffer(quadPatchInd, sizeof(quadPatchInd));

	if (strlen(m_Scene.heightMap))
		_loadTexture(GL_TEXTURE0, 0, m_Scene.heightMap, "TexTerrainHeight");
	if (strlen(m_Scene.normalMap))
		_loadTexture(GL_TEXTURE1, 1, m_Scene.normalMap, "TexTerrainNormal");
	if (strlen(m_Scene.texBase.colorMap))
		_loadTextureMipmap(GL_TEXTURE2, 2, m_Scene.texBase.mipLevels, m_Scene.texBase.colorMap, "TexBase");
	if (strlen(m_Scene.tex0.colorMap))
		_loadTextureMipmap(GL_TEXTURE3, 3, m_Scene.tex0.mipLevels, m_Scene.tex0.colorMap, "Tex0");
	if (strlen(m_Scene.tex1.colorMap))
		_loadTextureMipmap(GL_TEXTURE4, 4, m_Scene.tex1.mipLevels, m_Scene.tex1.colorMap, "Tex1");
	if (strlen(m_Scene.tex2.colorMap))
		_loadTextureMipmap(GL_TEXTURE5, 5, m_Scene.tex2.mipLevels, m_Scene.tex2.colorMap, "Tex2");
	if (strlen(m_Scene.tex3.colorMap))
		_loadTextureMipmap(GL_TEXTURE6, 6, m_Scene.tex3.mipLevels, m_Scene.tex3.colorMap, "Tex3");

	float tileWidth = VMB_TILE_WIDTH * VMB_PATCH_WIDTH;

	m_pShader->setUniform1f("TerrainWidth", tileWidth*m_Scene.terrainWidth);
	m_pShader->setUniform1f("TerrainHeight", tileWidth*m_Scene.terrainHeight);
	m_pShader->setUniform2f("TerrainOrigin", glm::vec2(
		-tileWidth * floor(m_Scene.terrainWidth / 2) - tileWidth / 2.0,
		-tileWidth * ceil(m_Scene.terrainHeight / 2) + tileWidth / 2.0));

	m_pShader->setUniform1f("ToggleWireFrame", 1.0);
	m_pShader->setUniform4f("SplatHeightSoft", glm::vec4(m_Scene.tex0.softHeight, m_Scene.tex1.softHeight, m_Scene.tex2.softHeight, m_Scene.tex3.softHeight));
	m_pShader->setUniform4f("SplatHeightHard", glm::vec4(m_Scene.tex0.hardHeight, m_Scene.tex1.hardHeight, m_Scene.tex2.hardHeight, m_Scene.tex3.hardHeight));

	// LIGHT
	struct Light
	{
		glm::vec4 origin;
		glm::vec3 color;
	} light0;

	light0.origin[0] = 100.0f;
	light0.origin[1] = 500.0f;
	light0.origin[2] = 0.0f;
	light0.origin[3] = 1.0f;
	light0.color[0] = 1.0f;
	light0.color[1] = 1.0f;
	light0.color[2] = 1.0f;

	m_pShader->setUniform4f("LightOrigin", light0.origin);
	m_pShader->setUniform3f("LightColor", light0.color);

	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));

}

StupidTerrain::StupidTerrain(const char * sceneFile, Camera * camera)
	:m_pCamera(camera)
{
	_loadScene(sceneFile);
	_initShaders();
	_initGraphics();
}

void StupidTerrain::render()
{
	m_pShader->bind();
	m_Vao.bind();
	m_IndexBuffer->bind();
	_drawTile(glm::vec3(0,0,0));
	//_drawTerrain(glm::vec3(m_pCamera->getPos().x, 0, m_pCamera->getPos().z), m_Scene.terrainWidth, m_Scene.terrainHeight);
}

void StupidTerrain::_drawTerrain(glm::vec3 vector, int width, int height)
{
	int row, col;

	for (row = 0; row < height; row++)
	{
		float r = vector.z + (VMB_TILE_WIDTH * VMB_PATCH_WIDTH * floor(height / 2.0)) - ((float)row * VMB_PATCH_WIDTH * VMB_TILE_WIDTH);

		for (col = 0; col < width; col++)
		{
			float c = vector.x - (VMB_TILE_WIDTH * VMB_PATCH_WIDTH * floor(width / 2.0)) + ((float)col * VMB_PATCH_WIDTH * VMB_TILE_WIDTH);
			_drawTile(glm::vec3(c, vector.y, r));
		}
	}
}

void StupidTerrain::_drawTile(glm::vec3 vector)
{
	int row, col;

	for (row = 0; row < VMB_TILE_WIDTH; row++)
	{
		float r = vector.z + (VMB_PATCH_WIDTH * 4.0) - ((float)row * VMB_PATCH_WIDTH) - (VMB_PATCH_WIDTH / 2.0);

		for (col = 0; col < VMB_TILE_WIDTH; col++)
		{
			float c = vector.x - (VMB_PATCH_WIDTH * 4.0) + ((float)col * VMB_PATCH_WIDTH) + (VMB_PATCH_WIDTH / 2.0);
			_drawPatch(glm::vec3(c, vector.y, r));
		}
	}
}

void StupidTerrain::_drawPatch(glm::vec3 vector)
{
	
	glm::mat4 g_mvMatrix = glm::mat4(1);
	glm::mat4 g_mMatrix = glm::mat4(1);
	g_mMatrix = glm::translate(vector);
	g_mvMatrix = m_pCamera->getViewMatrix() * g_mMatrix;
	//shader_updateMatrixBlock(VMB_MATBLOCK_MV_OFFSET_INDEX, 16 * sizeof(float), g_mvMatrix);
	//shader_updateMatrixBlock(VMB_MATBLOCK_M_OFFSET_INDEX, 16 * sizeof(float), g_mMatrix);
	m_pShader->setUniformMat4f("mvMatrix", g_mvMatrix);
	m_pShader->setUniformMat4f("mMatrix", g_mMatrix);
	
	// Calc normal matrix
	glm::mat3 g_nMatrix = glm::mat3(g_mvMatrix);
	g_nMatrix = glm::transpose(g_nMatrix);
	g_nMatrix = glm::inverse(g_nMatrix);

	m_pShader->setUniformMat3f("nMatrix", g_nMatrix);

	glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_SHORT, 0);

	
}

void StupidTerrain::_initScene()
{
	
	m_Scene.terrainHeight = 1.0;
	m_Scene.terrainWidth = 1.0;

	m_Scene.heightMap[0] = '\0';
	m_Scene.normalMap[0] = '\0';
	
	m_Scene.texBase.colorMap[0] = '\0';
	m_Scene.texBase.normalMap[0] = '\0';
	m_Scene.texBase.mipLevels = 3;
	m_Scene.texBase.softHeight = 0;
	m_Scene.texBase.hardHeight = 0;

	m_Scene.tex0.colorMap[0] = '\0';
	m_Scene.tex0.normalMap[0] = '\0';
	m_Scene.tex0.mipLevels = 3;
	m_Scene.tex0.softHeight = 0;
	m_Scene.tex0.hardHeight = 0;
	
	m_Scene.tex1.colorMap[0] = '\0';
	m_Scene.tex1.normalMap[0] = '\0';
	m_Scene.tex1.mipLevels = 3;
	m_Scene.tex1.softHeight = 0;
	m_Scene.tex1.hardHeight = 0;
	
	m_Scene.tex2.colorMap[0] = '\0';
	m_Scene.tex2.normalMap[0] = '\0';
	m_Scene.tex2.mipLevels = 3;
	m_Scene.tex2.softHeight = 0;
	m_Scene.tex2.hardHeight = 0;
	
	m_Scene.tex3.colorMap[0] = '\0';
	m_Scene.tex3.normalMap[0] = '\0';
	m_Scene.tex3.mipLevels = 3;
	m_Scene.tex3.softHeight = 0;
	m_Scene.tex3.hardHeight = 0;
}

void StupidTerrain::_initShaders()
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/terrain/stVertex.glsl"},
		{ GL_TESS_CONTROL_SHADER, "shaders/terrain/stTessControl.glsl" },
		{ GL_TESS_EVALUATION_SHADER, "shaders/terrain/stTessEvaluation.glsl" },
		{ GL_GEOMETRY_SHADER, "shaders/terrain/stGeometry.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/terrain/stFragment.glsl" },
		{ GL_NONE, NULL }
	};
	m_pShader = new Shader(shaders);

	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 200.0f);
	m_pShader->setUniformMat4f("pMatrix", projection);
	m_pShader->setUniform2f("Viewport", glm::vec2(1280, 720));
}

void StupidTerrain::_loadTexture(GLenum texture, int texNum, char * file,const char * uniform)
{
	
	std::cout << "Terrain:PNG:Loading " << file << "...";
	NS_TGALOADER::IMAGE image;
	
	if (image.LoadTGA(file))
	{
		printf("Texture loaded: %s\n", file);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glActiveTexture(texture);

		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, image.getWidth(), image.getHeight());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), image.getHeight(), GL_BGRA, GL_UNSIGNED_BYTE, image.getDataForOpenGL());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		m_pShader->setUniform1i(uniform, texNum);
	}
	else
	{
		printf("Error loading texture: %s\n", file);
	}
}

void StupidTerrain::_loadTextureMipmap(GLenum texture, int texNum, int mipLevels, char * file, const char * uniform)
{

	std::cout << "Terrain:PNG:Loading " << file << "...";
	NS_TGALOADER::IMAGE image;

	if (image.LoadTGA(file))
	{
		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GLCall(glActiveTexture(texture));


		GLuint texture;
		GLCall(glGenTextures(1, &texture));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, image.getWidth(), image.getHeight());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), image.getHeight(), GL_BGRA, GL_UNSIGNED_BYTE, image.getDataForOpenGL());
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

		m_pShader->setUniform1i(uniform, texNum);
		std::cout << "Done!\n";
	}
}
