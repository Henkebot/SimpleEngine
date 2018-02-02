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
	delete m_DiffuseMapTexture;
    delete[] m_Name;

}

void Material::bind(Shader* shader)
{
	int materialNr = 0;
	m_DiffuseMapTexture->bind(shader, materialNr++);

	m_AmbientTexture->bind(shader, materialNr++);

	if (m_SpecularTexture)
		m_SpecularTexture->bind(shader, materialNr++);
	else
	{
		m_SpecularTexture = m_AmbientTexture;
		m_SpecularTexture->bind(shader, materialNr++);
	}

	if (m_BumpTexture)
	{
		m_BumpTexture->bind(shader, materialNr++);
	}

	shader->setUniform3f("Materials.ambientColor", m_AmbientColor);
	shader->setUniform3f("Materials.diffuseColor", m_DiffuseColor);
	shader->setUniform3f("Materials.specularColor", m_SpecularColor);

}

void Material::unbind()
{
	m_DiffuseMapTexture->unbind();
}

void Material::setAmbientMap(const char * ambientMap)
{
	m_AmbientTexture = new Texture(ambientMap, "ambientTexture");
}

void Material::setDiffuseMap(const char * diffuseMap)
{
	m_DiffuseMapTexture = new Texture(diffuseMap, "diffuseTexture");
}

void Material::setSpecularMap(const char * specularMap)
{
	m_SpecularTexture = new Texture(specularMap, "specularTexture");
}

void Material::setBumpMap(const char * bumpMap)
{
	m_BumpTexture = new Texture(bumpMap, "bumpMap");
}

bool Material::operator==(const Material & other) const
{
	return !strcmp(m_Name, other.m_Name);
}
