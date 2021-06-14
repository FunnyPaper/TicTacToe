#include "Camera.h"

Camera::Camera(const glm::vec3& Position, const glm::vec3& Front, const glm::vec3& Up, float Pitch, float Yaw, float Fov, float Sensitivity)
	: m_Position(Position), m_Front(Front), m_Up(Up), m_Pitch(Pitch), m_Yaw(Yaw), m_Fov(Fov), m_Sensitivity(Sensitivity)
{
	UpdateViewMatrix();
}
Camera::Camera(float PositionX, float PositionY, float PositionZ, float FrontX, float FrontY, float FrontZ, float UpX, float UpY, float UpZ, float Pitch, float Yaw, float Fov, float Sensitivity) 
	: m_Pitch(Pitch), m_Yaw(Yaw), m_Fov(Fov), m_Sensitivity(Sensitivity)
{
	m_Position = glm::vec3(PositionX, PositionY, PositionZ);
	m_Front = glm::vec3(FrontX, FrontY, FrontZ);
	m_Up = glm::vec3(UpX, UpY, UpZ);
	UpdateViewMatrix();
}
void Camera::UpdateViewMatrix()
{
	//Ustaw kat widzenia
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(direction);
	//Ustaw polozenie
	m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}
void Camera::SendToShader(Shader* shader)
{
	shader->SetUniformVec3f("view_position", glm::value_ptr(m_Position));
	shader->SetUniformMat4f("view", glm::value_ptr(GetViewMatrix()));
	shader->SetUniformMat4f("projection", glm::value_ptr(GetProjectionMatrix()));
}