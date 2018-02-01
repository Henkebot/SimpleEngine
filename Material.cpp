#include "Material.h"

Material::Material(const char* name)
	: m_AmbientColor(glm::vec3(1)), m_DiffuseColor(glm::vec3(1)), m_SpecularColor(0.0f), m_Transparacy(0.0f), m_IllumMode(0), m_SurfaceDensity(0.0f),m_DiffuseMap(NULL), m_Name(name)
{

}

Material::~Material()
{
//	delete m_DiffuseMapTexture;
	//delete m_DiffuseMap;
//	delete m_Name;
}

void Material::bind()
{
	m_DiffuseMapTexture->bind(GL_TEXTURE0);
}

void Material::unbind()
{
	m_DiffuseMapTexture->unbind();
}

void Material::setDiffuseMap(const char * diffuseMap)
{
	m_DiffuseMap = diffuseMap;
	m_DiffuseMapTexture = new Texture(m_DiffuseMap);
}
