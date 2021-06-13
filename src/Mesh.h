#pragma once
#include "Vertex.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Primitives.h"

class Mesh
{
private:
	glm::vec3 m_Position{};
	glm::vec3 m_Rotation{};
	glm::vec3 m_Scale{};

	glm::mat4 m_ModelMatrix{};

	unsigned int VAO{}, VBO{}, EBO{};
	unsigned int m_NrOfIndices{}, m_NrOfVertices{};

	Primitives* _Temporary_Handle;

public:
	Mesh(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices = NULL, unsigned int NrOfIndices = 0, 
		const glm::vec3& Position = glm::vec3(0.0f), const glm::vec3& Rotation = glm::vec3(0.0f), const glm::vec3& Scale = glm::vec3(1.0f));

	Mesh(Primitives* primitive,
		const glm::vec3& Position = glm::vec3(0.0f), const glm::vec3& Rotation = glm::vec3(0.0f), const glm::vec3& Scale = glm::vec3(1.0f));

	~Mesh();

	void Render(Shader* shader);

	inline void SetPosition(glm::vec3 position) { m_Position = position; }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }

	inline void Move(glm::vec3 DirectionVector) { m_Position += DirectionVector; }
	inline void Rotate(glm::vec3 RotationVector) { m_Rotation += RotationVector; }
	inline void Scale(glm::vec3 ScaleVector) { m_Scale += ScaleVector; }

private:
	void InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices);
	void InitVAO(Primitives* primitive);
	void UpdateModelMatrix();
	const glm::mat3 GetNormalModelMatrix() { return glm::mat3(glm::transpose(glm::inverse(m_ModelMatrix))); }
	void UpdateUniforms(Shader* shader);
};

