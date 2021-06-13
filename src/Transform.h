#pragma once
#include "Shader.h"
#include <glm\glm.hpp>
#include <glm\mat4x4.hpp>
#include <glm\mat3x3.hpp>

class Transform
{
public:
	Transform(const glm::vec3& model_position, const glm::vec4& model_rotation, const glm::vec3& model_scale);
	Transform();
	void ChangePostion(float x, float y, float z) { m_Position = glm::vec3(x, y, z); UpdateModel(); }
	void ChangeRotation(float x, float y, float z, float angle) { m_Rotation = glm::vec4(x, y, z, angle); UpdateModel(); }
	void ChangeScale(float x, float y, float z) { m_Scale = glm::vec3(x, y, z); UpdateModel(); }

	void UpdateModel();
	const glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }
	const glm::mat3 GetNormalModelMatrix() { return glm::mat3(glm::transpose(glm::inverse(m_ModelMatrix))); }
	void SendToShader(Shader& shader);

private:
	glm::vec3 m_Position{};
	glm::vec4 m_Rotation{};
	glm::vec3 m_Scale{};
	glm::mat4 m_ModelMatrix{};
};

