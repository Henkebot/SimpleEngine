#ifndef TERRAIN_H
#define TERRAIN_H
#include <glm.hpp>
#include "../Shader.h"
#include "../Camera.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#define MAX_TERRAIN_NODES 1000

class Terrain
{
private:
	
	struct TerrainNode
	{
		glm::vec3 origin;
		float width;
		float height;
		int type;

		// Tessellation scale
		float tScaleNegX;
		float tScalePosX;
		float tScaleNegZ;
		float tScalePosZ;

		TerrainNode* parent;
		TerrainNode* child1;
		TerrainNode* child2;
		TerrainNode* child3;
		TerrainNode* child4;

		TerrainNode* north;
		TerrainNode* south;
		TerrainNode* east;
		TerrainNode* west;

	};
	TerrainNode* m_pTerrainTree;
	TerrainNode* m_pTerrainTail;
	int m_NumTerrainNodes;
	struct TerrainTexture
	{
		char colorMap[50];
		char normalMap[50];
		int mipLevels;

	};
	struct Scene
	{
		char heightMap[50];
		char normalMap[50];
		char blendMap[50];
		
		TerrainTexture texBase; // Base Texture
		TerrainTexture tex0; 
		TerrainTexture tex1; 
		TerrainTexture tex2; 
		TerrainTexture tex3;

		int terrainWidth;	// East/West in meters;
		int terrainLength;	// North/South in meters;
		int terrainHeight;	// HeightMap is mapped from 0 to this value (in meters)

	} scene;

	Shader* m_pShader;
	Camera* m_pCamera;
	VertexArray m_Vao;
	IndexBuffer* m_IndexBuffer;
	
public:
	static int VMB_TERRAIN_REC_CUTOFF;

	Terrain(const char* configFile, Camera* camera);
	void update();
	void render();

	inline Shader* getShader() { return m_pShader; }
private:

	void _createTree(float x, float y, float z, float width, float height);
	void _clearTree();

	bool _checkDivide(TerrainNode* node);
	GLboolean _divideNode(TerrainNode* node);
	TerrainNode* _createNode(TerrainNode* parent, int type, glm::vec3 origin, float width, float height);

	void _renderRecursive(TerrainNode* node);
	void _renderNode(TerrainNode* node);

	void _calcTessScale(TerrainNode* node);
	TerrainNode* find(TerrainNode* node, float x, float z);


	void _initShaders();
	void _initGraphics();
	void _initTerrain();
	void _initScene();
	void _loadTextureMipmap(GLenum texture, int texNum, int mipLevels, char* file, /* program, */ const char* uniform);
	void _loadConfig(const char* configFile);
};


#endif