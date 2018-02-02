#include "Material.h"

Material::Material(const char* name)
	: m_AmbientColor(glm::vec3(1)), m_DiffuseColor(glm::vec3(1)), m_SpecularColor(0.0f), m_Transparacy(0.0f), m_IllumMode(0), m_SurfaceDensity(0.0f), m_Name(name)
{
	m_DiffuseMapTexture = nullptr;
	m_AmbientTexture = nullptr;
	m_SpecularTexture = nullptr;
	m_BumpTexture = nullptr;
}

Material::~Material()
{
	if(m_DiffuseMapTexture)delete m_DiffuseMapTexture;
	if (m_AmbientTexture) delete m_AmbientTexture;
	if (m_SpecularTexture) delete m_SpecularTexture;
	if (m_BumpTexture) delete m_BumpTexture;
    
	delete m_Name;
}

void Material::bind(Shader* shader)
{
	int materialNr = 0;
	m_DiffuseMapTexture->bind(shader, materialNr++);

	if(m_AmbientTexture)
		m_AmbientTexture->bind(shader, materialNr++);

	if (m_SpecularTexture)
		m_SpecularTexture->bind(shader, materialNr++);

	if (m_BumpTexture)
	{
		m_BumpTexture->bind(shader, materialNr++);
	}

	shader->setUniform3f("ambientColor", m_AmbientColor);
	shader->setUniform3f("diffuseColor", m_DiffuseColor);
	shader->setUniform3f("specularColor", m_SpecularColor);

}

void Material::unbind()
{
	m_DiffuseMapTexture->unbind();
}

void Material::setAmbientMap(const char * ambientMap)
{
	m_AmbientTexture = new Texture(ambientMap, "ambientTexture", PNG);
}

void Material::setDiffuseMap(const char * diffuseMap)
{
	m_DiffuseMapTexture = new Texture(diffuseMap, "diffuseTexture", PNG);
}

void Material::setSpecularMap(const char * specularMap)
{
	m_SpecularTexture = new Texture(specularMap, "specularTexture", PNG);
}

void Material::setBumpMap(const char * bumpMap)
{
	m_BumpTexture = new Texture(bumpMap, "bumpMap", PNG);
}

bool Material::operator==(const Material & other) const
{
	return !strcmp(m_Name, other.m_Name);
}
