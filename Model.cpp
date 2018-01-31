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

Model::Model(const char * path, const char * objname, int lol)
{
	m_UsingNormals = true;
	m_UsingTextures = true;
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
		_loadObjContinoues(objFile, vertices, uvs, normals, name);
		std::vector<unsigned short> indices;
		for (int i = 0; i < materials.size(); i++)
		{
			if (strcmp(name, materials.at(i)->getName()) == 0)
			{
				std::cout << name << std::endl; 
				m_Materials.push_back(materials.at(i));
				break;
			}
		}
		_indexVBO(vertices, uvs, normals, indices, iVertices, iUvs, iNormals);
		// NOW WE CAN SEND THIS SHIT TO THE BUFFERS
		/* The problem right now is that we need to render each vao seperately and each time
		update the buffer with the constants for each vao. We need to also bind all the textures needed*/
		VertexArray* localVao = new VertexArray;
		localVao->addBuffer(new Buffer(iVertices), 0);
		localVao->addBuffer(new Buffer(iUvs), 1);
		localVao->addBuffer(new Buffer(iNormals), 2);

		IndexBuffer* iB = new IndexBuffer(indices);

		m_Vaos.push_back(localVao);
		m_Indexes.push_back(iB);
		//vertices.clear();
		//iVertices.clear();
		//normals.clear();
		//iNormals.clear();
		//uvs.clear();
		//iUvs.clear();
		//indices.clear();

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
			m_Materials[i]->bind();

			GLCall(glDrawElements(GL_TRIANGLES, m_Indexes[i]->getIndices(), GL_UNSIGNED_SHORT, NULL));

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
			char* name = new char[128];
			fscanf_s(mttlibFile, "%s", name, 128);	
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

bool Model::_loadObjContinoues(FILE * file, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, const char*& mtlName)
{
	static std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	static std::vector<glm::vec3> temp_vertices;
	static std::vector<glm::vec2> temp_uvs;
	static std::vector<glm::vec3> temp_normals;

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
				std::cout << mtlName << ": We hit the spot now reset and read the other" << std::endl;
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
