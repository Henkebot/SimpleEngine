#ifndef MODEL_H
#define MODEL_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include <map>
#include <vector>
#include <glm.hpp>


class Model
{
	
private:
	struct PackedVertex;

	bool m_UsingNormals;
	bool m_UsingTextures;

	Texture* m_Texture;
	Shader* m_Shader;
	VertexArray m_Vao;
	IndexBuffer* m_Index;

	std::vector<IndexBuffer*> m_Indexes;
	std::vector<VertexArray*> m_Vaos;
	std::vector<Material*> m_Materials;
public:
	Model(const char* obj, const char* texture = NULL);
	Model(const char* path, const char* objname, int lol);
	~Model();

	Shader* getShader();
	void draw();

private:

	std::vector<Material*> _getMaterialsFromFile(const char* path, const char* objname);

	bool _loadObj(const char* path, 
		std::vector<glm::vec3>& out_vertices, 
		std::vector<glm::vec2>& out_uvs, 
		std::vector<glm::vec3>& out_normals);

	bool _loadObjContinoues(FILE* file,
		std::vector<unsigned int>& vertexIndices,
		std::vector<unsigned int>& uvIndices,
		std::vector<unsigned int>& normalIndices,
		std::vector<glm::vec3>& tempVertices,
		std::vector<glm::vec2>& tempUvs,
		std::vector<glm::vec3>& tempNormals,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals,
		const char*& mtlName);

	void computeTangentBasis(
		const std::vector<glm::vec3> & vertices,
		const std::vector<glm::vec2> & uvs,
		const std::vector<glm::vec3> & normals,

		std::vector<glm::vec3> & tangents,
		std::vector<glm::vec3> & bitangents
	);

	bool _getSimilarVertexIndex_fast(
		PackedVertex & packed, std::map<PackedVertex, unsigned short>& vertexToOutIndex, unsigned short & result
	);

	void _indexVBO(std::vector<glm::vec3>& in_vertics,
		std::vector<glm::vec2>& in_uvs,
		std::vector<glm::vec3>& in_normals,
		std::vector<glm::vec3>& in_tangents,
		std::vector<glm::vec3>& in_bitangents,

		std::vector<unsigned short>& out_indices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals,
		std::vector<glm::vec3>& out_tangents,
		std::vector<glm::vec3>& out_bitangents);

};

#endif // !MODEL_H

