#include "Model.h"
#include <iostream>
#include <map>

struct Model::PackedVertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;

	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	}
};

Model::Model(const char * obj, const char* texture)
{
	

	std::vector<glm::vec3> vertices, iVertices, normals, iNormals;
	std::vector<glm::vec2> uvs, iUvs;

	_loadObj(obj, vertices, uvs, normals);
	
	std::vector<unsigned short> indices;
	_indexVBO(vertices, uvs, normals, indices, iVertices, iUvs, iNormals);

	if (m_UsingTextures)
	{
		m_Texture = new Texture(texture);
	}

	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER, "shaders/objVert.glsl"},
		{GL_FRAGMENT_SHADER, "shaders/objFrag.glsl" },
		{ GL_NONE, NULL }
	};

	m_Shader = new Shader(shaders);

	m_Vao.addBuffer(new Buffer(iVertices), 0);
	if(m_UsingTextures) m_Vao.addBuffer(new Buffer(iUvs), 1);
	if(m_UsingNormals) m_Vao.addBuffer(new Buffer(iNormals), 2);

	m_Index = new IndexBuffer(indices);
}

Model::~Model()
{
	delete m_Shader;
	delete m_Texture;
	delete m_Index;
}

Shader * Model::getShader()
{
	return m_Shader;
}

void Model::draw()
{
	m_Texture->bind(GL_TEXTURE0);
	m_Shader->bind();
	m_Vao.bind();
	m_Index->bind();

	GLCall(glDrawElements(GL_TRIANGLES, m_Index->getIndices(), GL_UNSIGNED_SHORT, NULL));

	m_Texture->unbind();
}

bool Model::_loadObj(const char * path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::cout << "\nModel: Reading(" << path << ")....";
	
	FILE* file;
	fopen_s(&file, path, "r");
	if (file == NULL)
	{
		std::cout << "Model: Failed to load (" << path << ")\n";
		return false;
	}
	while (1)
	{
		char line[128];
		int result = fscanf_s(file, "%s", line, sizeof(line));
		if (result == EOF)
			break;	

		if (strcmp(line, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(line, "vt") == 0)
		{
			m_UsingTextures = true;
			glm::vec2 uv;
			fscanf_s(file, "%f %f", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			m_UsingNormals = true;
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0)
		{
			
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			if (m_UsingTextures && m_UsingNormals)
			{
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
																	&vertexIndex[1], &uvIndex[1], &normalIndex[1],
																	&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					std::cout << "Model: Obj-format not supported\n";
					fclose(file);
					return false;
				}

			}
			else if (m_UsingNormals)
			{
				int matches = fscanf_s(file, "%d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1],
					&vertexIndex[2], &normalIndex[2]);
				
				if (matches == 1)
				{
					std::cout << "Model: Obj-format is stupid, no normals from here on!\n";
					

				}else if (matches != 6)
				{
					std::cout << "Model: Obj-format not supported\n";
					fclose(file);
					return false;
				}
			}
			else if (m_UsingTextures)
			{
				int matches = fscanf_s(file, "%d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0],
					&vertexIndex[1], &uvIndex[1],
					&vertexIndex[2], &uvIndex[2]);
				if (matches != 6)
				{
					std::cout << "Model: Obj-format not supported\n";
					fclose(file);
					return false;
				}
			}
			else
			{
				int matches = fscanf_s(file, "%d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
				if (matches != 3)
				{
					std::cout << "Model: Obj-format not supported\n";
					fclose(file);
					return false;
				}
			}

			
			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(vertexIndex[i]);
				if(m_UsingTextures)uvIndices.push_back(uvIndex[i]);
				if(m_UsingNormals)normalIndices.push_back(normalIndex[i]);
			}
		}
		else
		{
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertxIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertxIndex - 1];
		out_vertices.push_back(vertex);

		if (m_UsingTextures)
		{
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);
		}

		if (m_UsingNormals)
		{
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			out_normals.push_back(normal);
		}

	}

	fclose(file);
	std::cout << "Done!\n";
	return true;
}

bool Model::_getSimilarVertexIndex_fast(PackedVertex & packed, std::map<PackedVertex, unsigned short>& vertexToOutIndex, unsigned short & result)
{
	std::map<PackedVertex, unsigned short>::iterator it = vertexToOutIndex.find(packed);
	
	if (it == vertexToOutIndex.end())
		return false;
	
	result = it->second;
	return true;
}

void Model::_indexVBO(std::vector<glm::vec3>& in_vertics, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<unsigned short>& out_indices, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{

	std::map<Model::PackedVertex, unsigned short> VertexToOutIndex;
	
	for (unsigned int i = 0; i < in_vertics.size(); i++)
	{
		Model::PackedVertex packed;

		if(m_UsingNormals && m_UsingTextures)
			packed = { in_vertics[i],in_uvs[i],in_normals[i] };
		else	
			packed = { in_vertics[i] };
		


		unsigned short index;
		bool found = _getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{
			out_indices.push_back(index);
		}
		else
		{
			out_vertices.push_back(in_vertics[i]);
			if(m_UsingTextures) out_uvs.push_back(in_uvs[i]);
			if(m_UsingNormals) out_normals.push_back(in_normals[i]);
			
			unsigned short newIndex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newIndex);
			VertexToOutIndex[packed] = newIndex;
		}
	}
}
