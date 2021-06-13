#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\vec3.hpp>

Transform::Transform(const glm::vec3& model_position, const glm::vec4& model_rotation, const glm::vec3& model_scale)
	: m_Position(model_position), m_Rotation(model_rotation), m_Scale(model_scale), m_ModelMatrix(glm::mat4(1.0f)) 
{
	UpdateModel();
}
Transform::Transform()
	: m_Position(glm::vec3(1.0f)), m_Rotation(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)), m_Scale(glm::vec3(1.0f)), m_ModelMatrix(glm::mat4(1.0f))
{
	UpdateModel();
}

void Transform::UpdateModel()
{
	m_ModelMatrix = glm::mat4(1.0f);
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Rotation.w, glm::vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}

void Transform::SendToShader(Shader& shader)
{
	shader.SetUniformMat4f("model", glm::value_ptr(m_ModelMatrix));
	shader.SetUniformMat3f("model_normals", glm::value_ptr(GetNormalModelMatrix()));
}