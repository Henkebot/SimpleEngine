#include "Terrain.h"
#include "../TGALoader.h"
#include <vector>
#include <iostream>
#include "../OpenGLError.h"
int Terrain::VMB_TERRAIN_REC_CUTOFF = 20;
Terrain::Terrain(const char * configFile, Camera* camera)
	:m_pCamera(camera)
{
	_initScene();
	_loadConfig(configFile);
	_initTerrain();
	_initShaders();
	_initGraphics();

}
Terrain::~Terrain()
{
	delete[] m_pTerrainTree;
	delete m_pShader;
	delete m_IndexBuffer;
}
void Terrain::update()
{
	_createTree(0, 0, 0, scene.terrainWidth, scene.terrainLength);
}
void Terrain::render()
{
	m_pShader->bind();
	m_Vao.bind();
	m_IndexBuffer->bind();

	m_pShader->setUniformMat4f("View", m_pCamera->getViewMatrix());
	m_pShader->setUniform3f("camera_pos", m_pCamera->getPos());

	_renderRecursive(m_pTerrainTree);
}
void Terrain::_createTree(float x, float y, float z, float width, float height)
{
	_clearTree();

	m_pTerrainTree->type = 0;
	m_pTerrainTree->origin = glm::vec3(x,y,z);
	m_pTerrainTree->width = width;
	m_pTerrainTree->height = height;
	m_pTerrainTree->tScaleNegX = 1.0;
	m_pTerrainTree->tScaleNegZ = 1.0;
	m_pTerrainTree->tScalePosX = 1.0;
	m_pTerrainTree->tScalePosZ = 1.0;
	m_pTerrainTree->parent = m_pTerrainTree;

	_divideNode(m_pTerrainTree);

}
void Terrain::_clearTree()
{
	
	m_pTerrainTail = m_pTerrainTree;

	memset(m_pTerrainTree, 0, MAX_TERRAIN_NODES * sizeof(TerrainNode));
	m_NumTerrainNodes = 0;
}
bool Terrain::_checkDivide(TerrainNode * node)
{
	if (!node) return false;


	float distance = abs(
		sqrt(
			pow((m_pCamera->getPos().x) - node->origin.x, 2.0) +
			pow((m_pCamera->getPos().z) - node->origin.z, 2.0)
		)
	);

	float distance2 = sqrt(
		pow(0.5*node->width, 2.0) +
		pow(0.5*node->height, 2.0)
	);

	return !(distance > 3* distance2 || node->width < VMB_TERRAIN_REC_CUTOFF);
	
}

GLboolean Terrain::_divideNode(TerrainNode * node)
{
	float newWidth = 0.5 * node->width;
	float newHeight = 0.5 * node->height;

	glm::vec3 camPositionXZ = glm::vec3(m_pCamera->getPos().x,0, m_pCamera->getPos().z);
	glm::vec3 camDir = m_pCamera->getTarget();

	glm::vec3 childPos[4] =
	{
		glm::vec3(node->origin.x - 0.5 * newWidth, node->origin.y, node->origin.z - 0.5 * newHeight),
		glm::vec3(node->origin.x + 0.5 * newWidth, node->origin.y, node->origin.z - 0.5 * newHeight),
		glm::vec3(node->origin.x + 0.5 * newWidth, node->origin.y, node->origin.z + 0.5 * newHeight),
		glm::vec3(node->origin.x - 0.5 * newWidth, node->origin.y, node->origin.z + 0.5 * newHeight)
	};

	bool c1 = !(node->origin.x < camPositionXZ.x && node->origin.z < camPositionXZ.z && camDir.x > 0 && camDir.z > 0);
	bool c2 = !(node->origin.x > camPositionXZ.x && node->origin.z < camPositionXZ.z && camDir.x < 0 && camDir.z > 0);
	bool c3 = !(node->origin.x > camPositionXZ.x && node->origin.z > camPositionXZ.z && camDir.x < 0 && camDir.z < 0);
	bool c4 = !(node->origin.x < camPositionXZ.x && node->origin.z > camPositionXZ.z && camDir.x > 0 && camDir.z < 0);
	

	if (c1)
	{
		node->child1 = _createNode(node, 1, childPos[0], newWidth, newHeight);
		if(_checkDivide(node->child1)) _divideNode(node->child1);
	}
	if (c2)
	{
		node->child2 = _createNode(node, 2, childPos[1], newWidth, newHeight);
		if(_checkDivide(node->child2)) _divideNode(node->child2);
	}
	if (c3)
	{
		node->child3 = _createNode(node, 3, childPos[2], newWidth, newHeight);
		if (_checkDivide(node->child3)) _divideNode(node->child3);
	}
	if (c4)
	{
		node->child4 = _createNode(node, 4, childPos[3], newWidth, newHeight);
		if (_checkDivide(node->child4)) _divideNode(node->child4);
	}
	
	return true;
}
Terrain::TerrainNode * Terrain::_createNode(TerrainNode * parent, int type, glm::vec3 origin, float width, float height)
{
	if (m_NumTerrainNodes >= MAX_TERRAIN_NODES)
		return NULL;

	m_NumTerrainNodes++;

	m_pTerrainTail++;
	m_pTerrainTail->type = type;
	m_pTerrainTail->origin = origin;
	m_pTerrainTail->width = width;
	m_pTerrainTail->height = height;
	m_pTerrainTail->tScaleNegX = 1.0f;
	m_pTerrainTail->tScaleNegZ = 1.0f;
	m_pTerrainTail->tScalePosX = 1.0f;
	m_pTerrainTail->tScalePosZ = 1.0f;
	m_pTerrainTail->parent = parent;

	return m_pTerrainTail;
}

void Terrain::_renderRecursive(TerrainNode * node)
{

	if (!node->child1 && !node->child2 && !node->child3 && !node->child4)
	{
		_renderNode(node);
		
		return;
	}

	if (node->child1)
		_renderRecursive(node->child1);
	if (node->child2)
		_renderRecursive(node->child2);
	if (node->child3)
		_renderRecursive(node->child3);
	if (node->child4)
		_renderRecursive(node->child4);


}
void Terrain::_renderNode(TerrainNode * node)
{
	_calcTessScale(node);

	glm::mat4 g_mvMatrix;
	glm::mat4 g_mMatrix;
	g_mMatrix = glm::translate(node->origin);

	m_pShader->setUniformMat4f("World", g_mMatrix);

	// Send the size of the patch to the GPU
	m_pShader->setUniform1f("tileScale", 0.5*node->width);

	// Send patch neighbor edge tess scale factors
	m_pShader->setUniform1f("tscale_negx", node->tScaleNegX);
	m_pShader->setUniform1f("tscale_negz", node->tScaleNegZ);
	m_pShader->setUniform1f("tscale_posx", node->tScalePosX);
	m_pShader->setUniform1f("tscale_posz", node->tScalePosZ);

	GLCall(glDrawElements(GL_PATCHES, 4, GL_UNSIGNED_SHORT, 0));
}

void Terrain::_calcTessScale(TerrainNode * node)
{
	TerrainNode* t;
	
	// Positive Z(north)
	t = find(m_pTerrainTree, node->origin.x, node->origin.z + 1 + node->width / 2.0);
	if (t->width < node->width)
		node->tScalePosZ = node->width / t->width;
	
	// Positive X(east)
	t = find(m_pTerrainTree, node->origin.x + 1 + node->width / 2.0, node->origin.z);
	if (t->width < node->width)
		node->tScalePosX = node->width / t->width;

	// Negative Z(south)
	t = find(m_pTerrainTree, node->origin.x, node->origin.z - 1 - node->width / 2.0);
	if (t->width < node->width)
		node->tScaleNegZ = node->width / t->width;
	// Negative X(south)
	t = find(m_pTerrainTree, node->origin.x - 1 - node->width / 2.0, node->origin.z);
	if (t->width < node->width)
		node->tScaleNegX = node->width / t->width;
}

Terrain::TerrainNode * Terrain::find(TerrainNode * node, float x, float z)
{
	if (node->origin.x == x && node->origin.z == z)
		return node;

	if (node->origin.x >= x && node->origin.z >= z && node->child1)
		return find(node->child1, x, z);
	else if (node->origin.x <= x && node->origin.z >= z && node->child2)
		return find(node->child2, x, z);
	else if (node->origin.x <= x && node->origin.z <= z && node->child3)
		return find(node->child3, x, z);
	else if (node->origin.x >= x && node->origin.z <= z && node->child4)
		return find(node->child4, x, z);

	return node;
}
void Terrain::_initShaders()
{
	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/terrain/tVertex.glsl"},
		{GL_TESS_CONTROL_SHADER, "shaders/terrain/tTesCon.glsl" },
		{GL_TESS_EVALUATION_SHADER, "shaders/terrain/tTesEval.glsl" },
		{GL_GEOMETRY_SHADER, "shaders/terrain/tGeo.glsl" },
		{GL_FRAGMENT_SHADER, "shaders/terrain/tFrag.glsl" },
		{GL_NONE, NULL}
	};

	m_pShader = new Shader(shaders);
	static const GLfloat aspect = 1280.0f / 720.0f;
	static const glm::mat4 projection = glm::perspective(70.0f, aspect, 0.01f, 2000000.0f);
	m_pShader->setUniformMat4f("Projection", projection);
	

}
void Terrain::_initGraphics()
{
	// DATA SETUP
	float quadVert[] = {
		// Vert 1
		-1.0f, 0.0f, -1.0f, 1.0,	// Position
		// Vert 2
		1.0f, 0.0f, -1.0f, 1.0,		// Position	
		// Vert 3
		1.0f, 0.0f, 1.0f, 1.0,		// Position
		// Vert 4
		-1.0f, 0.0f, 1.0f, 1.0,		// Position	
	};
	float quadColors[] =
	{
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color
		0.18f, 0.91f, 0.46f, 1.0,	// Color
	};

	float quadNormals[] =
	{
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
		0.0f, 1.0f, 0.0f, 0.0f,		// Normal
	};
	
	float quadUVs[] =
	{
		0.0f, 1.0f,					// Tex coord (u,v)
		1.0f, 1.0f,					// Tex coord (u,v)
		1.0f, 0.0f,					// Tex coord (u,v)
		0.0f, 0.0f,					// Tex coord (u,v)
	};
	GLushort quadPatchIndices[] = { 0, 1, 2, 3 };

	m_Vao.addBuffer(new Buffer(quadVert, sizeof(quadVert), 4), 0);
	m_Vao.addBuffer(new Buffer(quadUVs, sizeof(quadUVs), 2), 1);
	m_IndexBuffer = new IndexBuffer(quadPatchIndices, sizeof(quadPatchIndices));

	// TEXTURE SETUP
	_loadTextureMipmap(GL_TEXTURE0, 0, 5, scene.heightMap, "TexTerrainHeight");
	_loadTextureMipmap(GL_TEXTURE1, 1, 5, scene.normalMap, "NormalMap");
	_loadTextureMipmap(GL_TEXTURE2, 2, 5, scene.blendMap, "BlendMap");
	_loadTextureMipmap(GL_TEXTURE3, 3, scene.texBase.mipLevels, scene.texBase.colorMap, "TexBase");
	_loadTextureMipmap(GL_TEXTURE4, 4, scene.texBase.mipLevels, scene.texBase.normalMap, "TexBaseNormal");
	_loadTextureMipmap(GL_TEXTURE5, 5, scene.tex0.mipLevels, scene.tex0.colorMap, "Tex0");
	_loadTextureMipmap(GL_TEXTURE6, 6, scene.tex1.mipLevels, scene.tex1.colorMap, "Tex1");
	_loadTextureMipmap(GL_TEXTURE7, 7, scene.tex2.mipLevels, scene.tex2.colorMap, "Tex2");

	m_pShader->setUniform1f("TerrainLength", scene.terrainLength);
	m_pShader->setUniform1f("TerrainWidth", scene.terrainWidth);
	m_pShader->setUniform3f("TerrainOrigin", glm::vec3(-scene.terrainWidth / 2.0f, 0.0, -scene.terrainLength / 2.0f));
	m_pShader->setUniform1f("TerrainHeightOffset", scene.terrainHeight);

	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));

}
void Terrain::_initTerrain()
{
	m_pTerrainTree = new TerrainNode[MAX_TERRAIN_NODES];// (TerrainNode*)malloc(MAX_TERRAIN_NODES * sizeof(TerrainNode));
	_clearTree();
}

void Terrain::_initScene()
{
	scene.terrainHeight = 1.0;
	scene.terrainWidth = 1.0;
	
	scene.heightMap[0] = '\0';
	scene.normalMap[0] = '\0';

	scene.texBase.colorMap[0] = '\0';
	scene.texBase.normalMap[0] = '\0';
	scene.texBase.mipLevels = 3;

	scene.tex0.colorMap[0] = '\0';
	scene.tex0.normalMap[0] = '\0';
	scene.tex0.mipLevels = 3;

	scene.tex1.colorMap[0] = '\0';
	scene.tex1.normalMap[0] = '\0';
	scene.tex1.mipLevels = 3;

	scene.tex2.colorMap[0] = '\0';
	scene.tex2.normalMap[0] = '\0';
	scene.tex2.mipLevels = 3;

	scene.tex3.colorMap[0] = '\0';
	scene.tex3.normalMap[0] = '\0';
	scene.tex3.mipLevels = 3;
}

void Terrain::_loadTextureMipmap(GLenum texture, int texNum, int mipLevels, char * file,/* program, */ const char * uniform)
{
	NS_TGALOADER::IMAGE image;
	std::cout << "Terrain: Loading " << file << "...";
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

void Terrain::_loadConfig(const char * configFile)
{
	_initScene();

	FILE* file;
	fopen_s(&file, configFile, "r");
	if (file == nullptr)
	{
		std::cout << "Terrain: Failed to open " << configFile << std::endl;
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
			scene.terrainWidth = value;
		}
		else if (!strcmp(line, "TerrainLength"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.terrainLength = value;
		}
		else if (!strcmp(line, "TerrainHeight"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.terrainHeight = value;
		}

		else if (!strcmp(line, "HeightMap"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.heightMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "NormalMap"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "BlendMap"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.blendMap, 50 * sizeof(char), location);
		}

		else if (!strcmp(line, "TexBase"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.texBase.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "TexBaseNormal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.texBase.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "TexBaseMipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.texBase.mipLevels = value;
		}

		else if (!strcmp(line, "Tex0"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex0.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex0Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex0.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex0MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.tex0.mipLevels = value;
		}

		else if (!strcmp(line, "Tex1"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex1.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex1Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex1.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex1MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.tex1.mipLevels = value;
		}

		else if (!strcmp(line, "Tex2"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex2.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex2Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex2.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex2MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.tex2.mipLevels = value;
		}

		else if (!strcmp(line, "Tex3"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex3.colorMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex3Normal"))
		{
			char location[50];
			fscanf_s(file, "%s", &location, 50);
			strcpy_s(scene.tex3.normalMap, 50 * sizeof(char), location);
		}
		else if (!strcmp(line, "Tex3MipLevels"))
		{
			float value = 0.0f;
			fscanf_s(file, "%f", &value);
			scene.tex3.mipLevels = value;
		}
		else
			std::cout << "Error reading " << configFile << std::endl;


	}
	
	fclose(file);

	if (scene.terrainHeight == 0.0)
		scene.terrainHeight = 1.0;
	if (scene.terrainWidth == 0.0)
		scene.terrainWidth = 1.0;

}
