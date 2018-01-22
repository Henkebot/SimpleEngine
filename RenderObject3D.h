#ifndef RENDER_OBJECT_3D_H
#define RENDER_OBJECT_3D_H

#include "RenderObject.h"


class RenderObject3D : public RenderObject
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Size;
	glm::vec3 m_Color;
public:
	RenderObject3D(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& size = glm::vec3(1.0f, 1.0f, 1.0f),
		const glm::vec3& rotation = glm::vec3(1.0f, 1.0f, 1.0f),
		const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));

	void updateUniforms() override;

};

#endif // !RENDER_OBJECT_3D

