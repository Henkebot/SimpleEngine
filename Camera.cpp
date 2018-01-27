#include "Camera.h"

Camera::Camera(int winWidth, int winHeight, const glm::vec3 & position, const glm::vec3 & target)
	: m_Width(winWidth), m_Height(winHeight), m_Position(position)
{
	m_Target = glm::normalize(target);

	m_Up = glm::vec3(0, 1, 0);

	m_Yaw = 0.45f;
	m_Pitch = 0.45f;

}

const glm::vec3 Camera::getPos() const
{
	return m_Position;
}

const glm::vec3 Camera::getTarget() const
{
	return m_Target;
}

const glm::vec3 Camera::getUp() const
{
	return m_Up;
}

const glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Target, m_Up);
}

void Camera::update(double xMouse, double yMouse)
{
	static float stepSize = 0.2f;
	const static int MARGIN = 10;

	if (GetAsyncKeyState(int('W')))
	{
		m_Position += m_Target * stepSize;
	}

	if (GetAsyncKeyState(int('S')))
	{
		m_Position -= m_Target * stepSize;
	}

	if (GetAsyncKeyState(int('A')))
	{
		glm::vec3 left = glm::cross(m_Target, m_Up);
		left = glm::normalize(left);
		left *= stepSize;
		m_Position -= left;
	}

	if (GetAsyncKeyState(int('D')))
	{
		glm::vec3 right = glm::cross(m_Target, m_Up);
		right = glm::normalize(right);
		right *= stepSize;
		m_Position += right;
	}

	float xOffset = xMouse - m_MousePos.x;
	float yOffset = m_MousePos.y - yMouse;
	m_MousePos.x = xMouse;
	m_MousePos.y = yMouse;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	
	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
	
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Target = glm::normalize(front);

}