#pragma once

#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"

namespace CAMERA_DEFAULT
{
	const float PITCH = 0.0f;
	const float YAW = -90.f;
	const float FOV = 45.0f;
	const float SENSITIVITY = 0.1f;
	const unsigned int WIDTH = 500;
	const unsigned int HEIGHT = 500;
}
enum class CAMERA_MOVEMENT
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
private:
	glm::vec3 m_Position, m_Front, m_Up, m_Right;
	float m_Yaw, m_Pitch, m_Sensitivity, m_Fov;
public:
	Camera(const glm::vec3& Position, const glm::vec3& Front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& Up = glm::vec3(0.0f, 1.0f, 0.0f), float Pitch = CAMERA_DEFAULT::PITCH, float Yaw = CAMERA_DEFAULT::YAW, float Fov = CAMERA_DEFAULT::FOV, float Sensitivity = CAMERA_DEFAULT::SENSITIVITY);
	Camera(float PositionX, float PositionY, float PositionZ, float FrontX = 0.0f, float FrontY = 0.0f, float FrontZ = -1.0f, float UpX = 0.0f, float UpY = 1.0f, float UpZ = 0.0f, float Pitch = CAMERA_DEFAULT::PITCH, float Yaw = CAMERA_DEFAULT::YAW, float Fov = CAMERA_DEFAULT::FOV, float Sensitivity = CAMERA_DEFAULT::SENSITIVITY);

	inline float GetZoom() const { return m_Fov; }
	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
	//inline glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(m_Fov), (float)CAMERA_DEFAULT::WIDTH / (float)CAMERA_DEFAULT::HEIGHT, 0.1f, 100.0f); }
	inline glm::mat4 GetProjectionMatrix() const { 
		return glm::ortho(
			-((float)CAMERA_DEFAULT::WIDTH / 200), (float)CAMERA_DEFAULT::WIDTH / 200, 
			-((float)CAMERA_DEFAULT::HEIGHT / 200), (float)CAMERA_DEFAULT::HEIGHT / 200, -1.0f, 1.0f); }
	inline glm::vec3 GetPosition() const { return m_Position; }

	void UpdateMovement(CAMERA_MOVEMENT move, float deltaTime);
	void UpdateMouse(float xoffset, float yoffset);
	void UpdateZoom(float yoffset);

	void SendToShader(Shader& shader);
private:
	void UpdateViewMatrix();
};

