#pragma once
#include <GL\glew.h>
#include "OpenGLError.h"
#include <glm.hpp>
#include <math.h>
#include <gtc/matrix_transform.hpp>
class ShadowMap
{
private:
	
	const unsigned int m_Width;
	const unsigned int m_Height;
	void _initDepthMap();
	unsigned int FBO, depthMap;
	glm::mat4 lightSpaceMatrix;

public:
	ShadowMap(unsigned int width, unsigned int height);
	void bindFBO();
	void unbindFBO();
	void bindDepthMap();
	void unbindDepthMap();
	const glm::mat4 getlightSpaceMatrix(glm::vec3 lightPos) const;
};
