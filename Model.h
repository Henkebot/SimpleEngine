#ifndef MODEL_H
#define MODEL_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include <map>
#include <vector>
#include <glm.hpp>


class Model
{
	
private:
	struct PackedVertex;
	bool m_UsingNormals;
	bool m_UsingTextures;
	Shader* m_Shader;
	VertexArray m_Vao;
	IndexBuffer* m_Index;
public:
	Model(const char* obj);
	~Model();

	Shader* getShader();
	void draw();

private:
	bool _loadObj(const char* path, 
		std::vector<glm::vec3>& out_vertices, 
		std::vector<glm::vec2>& out_uvs, 
		std::vector<glm::vec3>& out_normals);

	bool _getSimilarVertexIndex_fast(
		PackedVertex & packed, std::map<PackedVertex, unsigned short>& vertexToOutIndex, unsigned short & result
	);

	void _indexVBO(std::vector<glm::vec3>& in_vertics,
		std::vector<glm::vec2>& in_uvs,
		std::vector<glm::vec3>& in_normals,

		std::vector<unsigned short>& out_indices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

};

#endif // !MODEL_H

