#pragma once
// Wektory wielowymiarowe
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
// Shader
#include "Shader.h"

namespace CAMERA_DEFAULT
{
	// Przestrzeñ nazw do przechowywania ustawieñ kamery
	// Potrzebne w przypadku ruchomej kamery
	const float PITCH = 0.0f;
	const float YAW = -90.f;
	const float FOV = 45.0f;
	const float SENSITIVITY = 0.1f;
	// Kamera potrzebuje wymiary przestrzeni na której ma pracowaæ
	const unsigned int WIDTH = 500;
	const unsigned int HEIGHT = 500;
}

class Camera
{
	// Klasa kamery
private:
	// Wektory odpowiadaj¹ce za orientacjê kamery w przestrzeni 3D
	glm::vec3 m_Position, m_Front, m_Up, m_Right;
	// Zmienne przechowuj¹ce rotacjê, czu³oœæ poruszania i field of view
	float m_Yaw, m_Pitch, m_Sensitivity, m_Fov;

public:
	// Konstruktor kamery -> z wykorzystaniem wektorów
	Camera(const glm::vec3& Position, 
		   const glm::vec3& Front = glm::vec3(0.0f, 0.0f, -1.0f),
		   const glm::vec3& Up = glm::vec3(0.0f, 1.0f, 0.0f), 
		   float Pitch = CAMERA_DEFAULT::PITCH, 
		   float Yaw = CAMERA_DEFAULT::YAW, 
		   float Fov = CAMERA_DEFAULT::FOV, 
		   float Sensitivity = CAMERA_DEFAULT::SENSITIVITY);
	// Konstruktor kamery -> bez wektorów (mo¿na podaæ pamatery po przecinku)
	Camera(float PositionX, float PositionY, float PositionZ, 
		   float FrontX = 0.0f, float FrontY = 0.0f, float FrontZ = -1.0f, 
		   float UpX = 0.0f, float UpY = 1.0f, float UpZ = 0.0f, 
		   float Pitch = CAMERA_DEFAULT::PITCH, 
		   float Yaw = CAMERA_DEFAULT::YAW, 
		   float Fov = CAMERA_DEFAULT::FOV, 
		   float Sensitivity = CAMERA_DEFAULT::SENSITIVITY);
	// Zwróæ FOV
	inline float GetZoom() const { return m_Fov; }
	// Zwróæ ViewMatrix -> macierz odpowiedzialna na k¹t widzenia (rotacja kamery w przestrzeni 3D)
	inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
	// Zwróæ ProjectionMatrix -> macierz odpowiedzialna za postrzeganie obrazu (ustawienia wzglêdem viewpointu)
	inline glm::mat4 GetProjectionMatrix() const;
	// Zwróæ pozycjê kamery w przestrzeni 3D
	inline glm::vec3 GetPosition() const { return m_Position; }
	// Wyœlij ustawienia do shadera
	void SendToShader(Shader* shader);

private:
	// Zaktualizuj ViewMatrix
	void UpdateViewMatrix();
};

