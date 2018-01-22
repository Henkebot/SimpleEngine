#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx\rotate_vector.hpp>
#include <math.h>
#include <Windows.h>

class Camera
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Up;

	int m_Width;
	int m_Height;
	
	float m_Yaw;
	float m_Pitch;
	glm::vec2 m_MousePos;
public:
	Camera(int winWidth, int winHeight, const glm::vec3& position, const glm::vec3& target);
	
	const glm::vec3 getPos() const;
	const glm::vec3 getTarget() const;
	const glm::vec3 getUp() const;

	void update(double xMouse, double yMouse);
private:
	void Init();
};

#endif // !CAMERA_H

