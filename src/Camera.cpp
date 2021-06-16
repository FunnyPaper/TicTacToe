#include "Camera.h"

Camera::Camera(const glm::vec3& Position, 
	const glm::vec3& Front, 
	const glm::vec3& Up, 
	float Pitch, float Yaw, float Fov, 
	float Sensitivity)
	: m_Position(Position), 
	m_Front(Front), 
	m_Up(Up), 
	m_Pitch(Pitch), m_Yaw(Yaw), m_Fov(Fov), 
	m_Sensitivity(Sensitivity)
{
	// Konstruktor kamery
	// Zaktualizuj ViewMatrix -> odpowiednie ustawienie widoku w oknie kontekstowym
	UpdateViewMatrix();
}
Camera::Camera(float PositionX, float PositionY, float PositionZ, 
	float FrontX, float FrontY, float FrontZ, 
	float UpX, float UpY, float UpZ, 
	float Pitch, float Yaw, float Fov, 
	float Sensitivity) 
	: m_Pitch(Pitch), m_Yaw(Yaw), m_Fov(Fov), 
	m_Sensitivity(Sensitivity)
{
	// Konstruktor kamery
	// Stwórz i przypisz wektory: pozycyjny, zwrotowy i orientacyjny (position, front, up)
	// Niezbêdne do opisania kamery w przestrzeni 3D -> pozycja, gdzie jest przód, gdzie jest góra
	m_Position = glm::vec3(PositionX, PositionY, PositionZ);
	m_Front = glm::vec3(FrontX, FrontY, FrontZ);
	m_Up = glm::vec3(UpX, UpY, UpZ);
	// Zaktualizuj ViewMatrix -> odpowiednie ustawienie widoku w oknie kontekstowym
	UpdateViewMatrix();
}
void Camera::UpdateViewMatrix()
{
	// Ustaw kat widzenia w oparciu o K¹ty Eulera
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(direction);
	// Policz gdzie jest "prawo" od kamery
	// Po tym kamera jest w pe³ni opisana przestrzennie
	m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}
glm::mat4 Camera::GetProjectionMatrix() const
{
	// Zwróæ widok ortogonalny kamery
	// Potrzebne jest okreœlenie gdzie znajduj¹ siê krawêdzie viewportu oraz zasiêg g³êbi (jak daleko nale¿y renderowaæ w przód i w ty³)
	return glm::ortho(
		-((float)CAMERA_DEFAULT::WIDTH / 200),
		(float)CAMERA_DEFAULT::WIDTH / 200,
		-((float)CAMERA_DEFAULT::HEIGHT / 200),
		(float)CAMERA_DEFAULT::HEIGHT / 200,
		-1.0f, 1.0f);
}
void Camera::SendToShader(Shader* shader)
{
	// Wyœlij pozycjê kamery oraz dwie macierze do shadera 
	shader->SetUniformVec3f("view_position", glm::value_ptr(m_Position));
	shader->SetUniformMat4f("view", glm::value_ptr(GetViewMatrix()));
	shader->SetUniformMat4f("projection", glm::value_ptr(GetProjectionMatrix()));
}