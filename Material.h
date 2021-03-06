#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL\glew.h>
#include <glm.hpp>
#include "Texture.h"
#include "Shader.h"

class Material
{
private:
	// After newmtll
	const char* m_Name;
	// Ka
	glm::vec3 m_AmbientColor;
	// Kd
	glm::vec3 m_DiffuseColor;
	// Ks
	glm::vec3 m_SpecularColor;
	// Ns
	float m_SpecularExponent;
	// d
	float m_Transparacy;
	// illum
	int m_IllumMode;
	// Tf (Which color channel who should pass)
	glm::vec3 m_TransmissionFilter;
	// Ni (How the light refracts thro the surface, water have 1.5)
	float m_SurfaceDensity;
	//map_Kd(Texture)
	Texture* m_DiffuseMapTexture;
	//map_Ka 
	Texture* m_AmbientTexture;
	//map_Ks
	Texture* m_SpecularTexture;
	//map_Bump
	Texture* m_BumpTexture;


public:
	Material(const char* name);
	~Material();
	
	void bind(Shader* shader);
	void unbind();
	
	// Ambient 
	inline void setAmbientColor(float r, float g, float b) { m_AmbientColor = glm::vec3(r, g, b); }
	void setAmbientMap(const char* ambientMap);
	
	// Diffuse
	inline void setDiffuseColor(float r, float g, float b) { m_DiffuseColor = glm::vec3(r, g, b); }
	void setDiffuseMap(const char* diffuseMap);
	
	// Specular
	inline void setSpecularColor(float r, float g, float b) { m_SpecularColor = glm::vec3(r, g, b); }
	inline void setSpecularExponent(float exp) { m_SpecularExponent = exp; }
	void setSpecularMap(const char* specularMap);

	// Bump Map
	void setBumpMap(const char* bumpMap);

	inline void setTranspararacy(float trans) { m_Transparacy = trans; }

	inline void setIllumMode(int mode) { m_IllumMode = mode; }

	inline void setTransmissionFilter(float r, float g, float b) { m_TransmissionFilter = glm::vec3(r, g, b); }

	inline void setSurfaceDensity(float density) { m_SurfaceDensity = density; }

	inline const char* getName() const { return m_Name; }

	bool operator==(const Material& other) const;


		
};

#endif // !MATERIAL_H

