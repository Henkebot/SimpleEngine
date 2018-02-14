//#include "stdafx.h"
#include "ShadowMap.h"

void ShadowMap::_initDepthMap()
{
	glGenFramebuffers(1, &FBO);
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0,1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::ShadowMap(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height)
{
	_initDepthMap();
		 
}

void ShadowMap::bindFBO()
{
	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

}

void ShadowMap::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::bindDepthMap()
{
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

void ShadowMap::unbindDepthMap()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const glm::mat4 ShadowMap::getlightSpaceMatrix(glm::vec3 lightPos) const
{
	float near_plane = 1.0f, far_plane = 100.5f;
	//glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)m_Width / (GLfloat)m_Height, near_plane, far_plane);
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, 10.0f, -10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	return lightProjection * lightView;
}
