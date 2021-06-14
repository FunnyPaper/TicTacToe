#include "Mesh.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>


Mesh::Mesh(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices,
	const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale)
		: m_NrOfIndices(NrOfIndices), m_NrOfVertices(NrOfVertices), _Temporary_Handle(NULL)
{
	m_Position = Position;
	m_Rotation = Rotation;
	m_Scale = Scale;

	InitVAO(Vertices, NrOfVertices, Indices, NrOfIndices);
	UpdateModelMatrix();
}

Mesh::Mesh(Primitives* primitive,
	const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale) : _Temporary_Handle(primitive)
{
	m_Position = Position;
	m_Rotation = Rotation;
	m_Scale = Scale;

	m_NrOfIndices = primitive->GetIndicesNumber();
	m_NrOfVertices = primitive->GetVerticesNumber();

	InitVAO(primitive);
	UpdateModelMatrix();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete _Temporary_Handle;
}

void Mesh::InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, NrOfVertices * sizeof(Vertex), Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NrOfIndices * sizeof(unsigned int), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}
void Mesh::InitVAO(Primitives* primitive)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_NrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NrOfIndices * sizeof(unsigned int), primitive->GetIndices(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}

void Mesh::UpdateModelMatrix()
{
	m_ModelMatrix = glm::mat4(1.0f);

	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}

void Mesh::UpdateUniforms(Shader* shader)
{
	shader->SetUniformMat4f("model", glm::value_ptr(m_ModelMatrix));
	shader->SetUniformMat3f("model_normals", glm::value_ptr(GetNormalModelMatrix()));
}

void Mesh::Render(Shader* shader)
{
	UpdateModelMatrix();
	UpdateUniforms(shader);

	glBindVertexArray(VAO);

	if (m_NrOfIndices)
		glDrawElements(GL_TRIANGLES, m_NrOfIndices, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0, m_NrOfVertices);
}