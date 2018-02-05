#ifndef STUPID_TERRAIN_H
#define STUPID_TERRAIN_H
#include "../Shader.h"
#include "../Camera.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#define VMB_PATCH_WIDTH 40.0
#define VMB_TILE_WIDTH 8 // 1 tile = 8 x 8 patches
class StupidTerrain
{
private:
	struct tex_terrain
	{
		char colorMap[50];
		char normalMap[50];
		int mipLevels;
		float softHeight;
		float hardHeight;
	};

	struct Scene
	{
		char heightMap[50];
		char normalMap[50];

		tex_terrain texBase; // Base texture.
		tex_terrain tex0;
		tex_terrain tex1;
		tex_terrain tex2;
		tex_terrain tex3;

		int terrainWidth;  // Number of tiles
		int terrainHeight; // Number of tiles
	}m_Scene;

	Shader* m_pShader;
	VertexArray m_Vao;
	IndexBuffer* m_IndexBuffer;
	
	Camera* m_pCamera;
public:

	StupidTerrain(const char * sceneFile, Camera * camera);
	void render();
private:
	void _drawTerrain(glm::vec3 vector, int width, int height);
	void _drawTile(glm::vec3 vector);
	void _drawPatch(glm::vec3 vector);
	void _initScene();
	void _initShaders();
	void _loadTexture(GLenum texture, int texNum, char* file, const char* uniform);
	void _loadTextureMipmap(GLenum texture, int texNum, int mipLevels, char* file, const char* uniform);
	int _loadScene(const char* sceneFile);
	void _initGraphics();

};

#endif // !STUPID_TERRAIN_H
