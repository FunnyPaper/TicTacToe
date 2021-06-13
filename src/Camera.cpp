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
void Camera::UpdateMovement(CAMERA_MOVEMENT move, float deltaTime)
{
	const float velocity = 2.5f * deltaTime;
	// Przeksztalc vectory polozenia
	if (move == CAMERA_MOVEMENT::FORWARD)
		m_Position += velocity * m_Front;
	if (move == CAMERA_MOVEMENT::BACKWARD)
		m_Position -= velocity * m_Front;
	if (move == CAMERA_MOVEMENT::LEFT)
		m_Position -= velocity * m_Right;
	if (move == CAMERA_MOVEMENT::RIGHT)
		m_Position += velocity * m_Right;
	if (move == CAMERA_MOVEMENT::UP)
		m_Position += velocity * m_Up;
	if (move == CAMERA_MOVEMENT::DOWN)
		m_Position -= velocity * m_Up;
}
void Camera::UpdateMouse(float xoffset, float yoffset)
{
	// Przeksztalc dane odpowiedzialne za kat widzenia
	m_Pitch += yoffset * m_Sensitivity;
	m_Yaw += xoffset * m_Sensitivity;

	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	else if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	UpdateViewMatrix();
}
void Camera::UpdateZoom(float yoffset)
{
	m_Fov -= yoffset;
	if (m_Fov < 1.0f) m_Fov = 1.0f;
	else if (m_Fov > 45.0f) m_Fov = 45.0f;
}
void Camera::SendToShader(Shader& shader)
{
	shader.SetUniformVec3f("view_position", glm::value_ptr(m_Position));
	shader.SetUniformMat4f("view", glm::value_ptr(GetViewMatrix()));
	shader.SetUniformMat4f("projection", glm::value_ptr(GetProjectionMatrix()));
}