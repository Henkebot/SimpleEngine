#include "Model.h"
#include <iostream>
#include <string>
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

	/*std::vector<glm::vec3> vertices, iVertices, normals, iNormals;
	std::vector<glm::vec2> uvs, iUvs;

	_loadObj(obj, vertices, uvs, normals);
	
	std::vector<unsigned short> indices;
	_indexVBO(vertices, uvs, normals, indices, iVertices, iUvs, iNormals);

	if (m_UsingTextures)
	{
		m_Texture = new Texture(texture, texture);
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

	m_Index = new IndexBuffer(indices);*/
}

Model::Model(const char * path, const char * objname, int lol)
{

	std::vector<Material*> materials =  _getMaterialsFromFile(path, objname);
	if (materials.size() == 0)
	{
		std::cout << "ERROR:Model: Failed to load materials!\n";
	}
	// materials.size() is how many meshes we will have.
	// We will now now read the vertices, textures and normals
	// using _loadObjContinues
	FILE* objFile;
	std::string finalPath = path;
	finalPath += "/";
	finalPath += objname;
	int nrOf = 0;
	fopen_s(&objFile, finalPath.c_str(), "r");
	if (objFile == NULL)
	{
		std::cout << "ERROR:Model: Failed to read obj file" << finalPath << std::endl;
	}

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	while (1)
	{
		char line[128];
		int result = fscanf_s(objFile, "%s", line, sizeof(line));
		if (result == EOF)
		{
			break;
		}
		std::vector<glm::vec3> vertices, iVertices, normals, iNormals;
		std::vector<glm::vec2> uvs, iUvs;
		const char* name = nullptr;
		// Load Obj
		_loadObjContinoues(objFile, vertexIndices, uvIndices, normalIndices, temp_vertices, temp_uvs, temp_normals, vertices, uvs, normals, name);
		// Find that material which is being used in the mesh
		for (int i = 0; i < materials.size(); i++)
		{
			if (name == NULL) break;
			if (strcmp(name, materials.at(i)->getName()) == 0)
			{
				m_Materials.push_back(materials.at(i));
				break;
			}
		}
		delete[] name;

		// Calculate tangents and bitangents.
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<glm::vec3> indexed_tangents;
		std::vector<glm::vec3> indexed_bitangents;
		
		computeTangentBasis(vertices, uvs, normals, tangents, bitangents);
		
		std::vector<unsigned short> indices;

		_indexVBO(vertices, uvs, normals, tangents, bitangents, indices, iVertices, iUvs, iNormals, indexed_tangents, indexed_bitangents);
		// NOW WE CAN SEND THIS SHIT TO THE BUFFERS
		/* The problem right now is that we need to render each vao seperately and each time
		update the buffer with the constants for each vao. We need to also bind all the textures needed*/
		VertexArray* localVao = new VertexArray;
		localVao->addBuffer(new Buffer(iVertices), 0);
		localVao->addBuffer(new Buffer(iUvs), 1);
		localVao->addBuffer(new Buffer(iNormals), 2);
		localVao->addBuffer(new Buffer(indexed_tangents), 3);
		localVao->addBuffer(new Buffer(indexed_bitangents), 3);

		IndexBuffer* iB = new IndexBuffer(indices);

		m_Vaos.push_back(localVao);
		m_Indexes.push_back(iB);
	}

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/objVert.glsl" },
		{ GL_FRAGMENT_SHADER, "shaders/objFrag.glsl" },
		{ GL_NONE, NULL }
	};

	m_Shader = new Shader(shaders);


}

Model::~Model()
{
	delete m_Shader;
	delete m_Texture;
	delete m_Index;

	for (auto& index : m_Indexes)
		delete index;
	for (auto& vertx : m_Vaos)
		delete vertx;


	// Safely delete materials if there are copies
	for (int i = 0; i < m_Materials.size(); i++)
	{
		bool found = false;
		for (int k = i + 1; k < m_Materials.size() - 1; k++)
		{
			found = *m_Materials.at(i) == *m_Materials.at(k);
			if (found) break;
		}
		if (!found)
		{
			delete m_Materials.at(i);
			m_Materials.erase(m_Materials.begin() + i);
			i = 0;
		}
	}


}

Shader * Model::getShader()
{
	return m_Shader;
}

void Model::draw()
{
		m_Shader->bind();
		for (int i = 0; i < m_Vaos.size(); i++)
		{
			m_Vaos[i]->bind();
			m_Indexes[i]->bind();
			m_Materials[i]->bind(m_Shader);

			GLCall(glDrawElements(GL_TRIANGLES, m_Indexes[i]->getIndices() + 1, GL_UNSIGNED_SHORT, NULL));

			m_Materials[i]->unbind();

		}
}

std::vector<Material*> Model::_getMaterialsFromFile(const char * path, const char * objname)
{

	std::vector<Material*> materials;
	FILE* objFile;
	char mttlibName[64] = { 0 };

	std::string finalPath = path;
	finalPath += "/";
	finalPath += objname;

	fopen_s(&objFile, finalPath.c_str(), "r");

	if (objFile == NULL)
	{
		std::cout << "ERROR:Model: Failed to read obj file" << finalPath << std::endl;
		return materials;
	}

	while (1)
	{
		char line[128] = {0};
		if (fscanf_s(objFile, "%s", line, sizeof(line)) == EOF)
		{
			std::cout << "ERROR:Model: Failed to find mtllib in obj file" << finalPath << std::endl;
			fclose(objFile);
			return materials;
		}
		if (strcmp("mtllib", line) == 0)
		{
			fscanf_s(objFile, "%s", mttlibName, sizeof(mttlibName));
			fclose(objFile);
			break;

		}
	}

	finalPath = path;
	finalPath += "/";
	finalPath += mttlibName;
	FILE* mttlibFile;
	fopen_s(&mttlibFile, finalPath.c_str(), "r");
	
	if (mttlibFile == NULL)
	{
		std::cout << "ERROR:Model: Failed to open mtllib file" << finalPath << std::endl;
		fclose(mttlibFile);
		return materials;
	}
	while (1)
	{
		char line[128];
		int result = fscanf_s(mttlibFile, "%s", line, sizeof(line));
		if (result == EOF)
		{
			break;
		}

		if (strcmp("newmtl", line) == 0)
		{
			char* name = new char[64];
			fscanf_s(mttlibFile, "%s", name, 64);	
			std::cout << "Model:Mttlib: Reading (" << name << ")\n";
			materials.push_back(new Material(name));
		}
		else if (strcmp("Ns", line) == 0)
		{
			float Ns = 0.0f;
			fscanf_s(mttlibFile, "%f", &Ns);
			materials.back()->setSpecularExponent(Ns);
		}
		else if (strcmp("Ni", line) == 0)
		{
			float Ni = 0.0f;
			fscanf_s(mttlibFile, "%f", &Ni);
			materials.back()->setSurfaceDensity(Ni);
		}
		else if (strcmp("d", line) == 0)
		{
			float d = 0.0f;
			fscanf_s(mttlibFile, "%f", &d);
			materials.back()->setTranspararacy(d);
		}
		else if (strcmp("Tf", line) == 0)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			fscanf_s(mttlibFile, "%f %f %f", &r, &g, &b);
			materials.back()->setTransmissionFilter(r, g, b);
		}
		else if (strcmp("illum", line) == 0)
		{
			int illum = 0; 
			fscanf_s(mttlibFile, "%i", &illum);
			materials.back()->setIllumMode(illum);
		}
		else if (strcmp("Ka", line) == 0)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			fscanf_s(mttlibFile, "%f %f %f", &r, &g, &b);
			materials.back()->setAmbientColor(r, g, b);
		}
		else if (strcmp("Kd", line) == 0)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			fscanf_s(mttlibFile, "%f %f %f", &r, &g, &b);
			materials.back()->setDiffuseColor(r, g, b);
		}
		else if (strcmp("Ks", line) == 0)
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			fscanf_s(mttlibFile, "%f %f %f", &r, &g, &b);
			materials.back()->setSpecularColor(r, g, b);
		}
		else if (strcmp("map_Kd", line) == 0)
		{
			char mapKd[128];
			fscanf_s(mttlibFile, "%s", mapKd, sizeof(mapKd));

			std::string location = path;
			location += "/";
			location += mapKd;
			materials.back()->setDiffuseMap(location.c_str());
		}
		else if (strcmp("map_Ka", line) == 0)
		{
			char mapKa[128];
			fscanf_s(mttlibFile, "%s", mapKa, sizeof(mapKa));

			std::string location = path;
			location += "/";
			location += mapKa;
			materials.back()->setAmbientMap(location.c_str());
		}
		else if (strcmp("map_Ks", line) == 0)
		{
			char mapKs[128];
			fscanf_s(mttlibFile, "%s", mapKs, sizeof(mapKs));

			std::string location = path;
			location += "/";
			location += mapKs;
			materials.back()->setSpecularMap(location.c_str());
		}
		else if (strcmp("map_Bump", line) == 0)
		{
			char mapBump[128];
			fscanf_s(mttlibFile, "%s", mapBump, sizeof(mapBump));

			std::string location = path;
			location += "/";
			location += mapBump;
			materials.back()->setBumpMap(location.c_str());
		}

	

	}


	return materials;

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

bool Model::_loadObjContinoues(FILE* file,
	std::vector<unsigned int>& vertexIndices,
	std::vector<unsigned int>& uvIndices,
	std::vector<unsigned int>& normalIndices,
	std::vector<glm::vec3>& temp_vertices,
	std::vector<glm::vec2>& temp_uvs,
	std::vector<glm::vec3>& temp_normals,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	const char*& mtlName)
{

	bool loadFace = false;
	bool loadVert = false;
	bool loadNormal = false;
	bool loadUV = false;
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
			glm::vec2 uv;
			fscanf_s(file, "%f %f", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(line, "f") == 0)
		{
			loadFace = true;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				std::cout << "Model: Obj-format not supported\n";
				fclose(file);
				return false;
			}

			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
		else if (strcmp(line, "usemtl") == 0)
		{
			const char* mtlNameTemp = new char[128];
			fscanf_s(file, "%s", mtlNameTemp, 128);
			mtlName = mtlNameTemp;
		}
		else
		{
			// If we have been reading faces and now hit a blank line does that mean that we are done with this mesh
			if (loadFace)
			{
				break;
			}
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	for (size_t i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertxIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertxIndex - 1];
		out_vertices.push_back(vertex);
		
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

	return true;
}

void Model::computeTangentBasis(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs, const std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
	for (unsigned int i = 0; i < vertices.size(); i += 3)
	{
		glm::vec3 v0 = vertices[i + 0];
		glm::vec3 v1 = vertices[i + 1];
		glm::vec3 v2 = vertices[i + 2];

		glm::vec2 uv0 = uvs[i + 0];
		glm::vec2 uv1 = uvs[i + 1];
		glm::vec2 uv2 = uvs[i + 2];

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent;
		tangent.x = r * (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x);
		tangent.y = r * (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y);
		tangent.z = r * (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z);
		tangent = glm::normalize(tangent);
		
		glm::vec3 bitangent;
		bitangent.x = r * (-deltaUV2.x * deltaPos1.x + deltaUV1.x * deltaPos2.x);
		bitangent.y = r * (-deltaUV2.x * deltaPos1.y + deltaUV1.x * deltaPos2.y);
		bitangent.z = r * (-deltaUV2.x * deltaPos1.z + deltaUV1.x * deltaPos2.z);
		bitangent = glm::normalize(bitangent);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);



	}
}

bool Model::_getSimilarVertexIndex_fast(PackedVertex & packed, std::map<PackedVertex, unsigned short>& vertexToOutIndex, unsigned short & result)
{
	std::map<PackedVertex, unsigned short>::iterator it = vertexToOutIndex.find(packed);
	
	if (it == vertexToOutIndex.end())
		return false;
	
	result = it->second;
	return true;
}

void Model::_indexVBO(std::vector<glm::vec3>& in_vertics,
	std::vector<glm::vec2>& in_uvs,
	std::vector<glm::vec3>& in_normals,
	std::vector<glm::vec3>& in_tangents,
	std::vector<glm::vec3>& in_bitangents,

	std::vector<unsigned short>& out_indices,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec3>& out_tangents,
	std::vector<glm::vec3>& out_bitangents)
{

	std::map<Model::PackedVertex, unsigned short> VertexToOutIndex;
	
	for (unsigned int i = 0; i < in_vertics.size(); i++)
	{
		Model::PackedVertex packed = { in_vertics[i],in_uvs[i],in_normals[i] };
		
		unsigned short index;
		bool found = _getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{
			out_indices.push_back(index);

			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
		}
		else
		{
			out_vertices.push_back(in_vertics[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_tangents.push_back(in_tangents[i]);
			out_bitangents.push_back(in_bitangents[i]);
			unsigned short newIndex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newIndex);
			VertexToOutIndex[packed] = newIndex;
		}
	}
}
