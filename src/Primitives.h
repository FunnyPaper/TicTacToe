#pragma once
#include "Vertex.h"
#include <vector>
#include <glm\glm.hpp>

class Primitives
{
protected:
	std::vector<Vertex> m_VertexData{};
	std::vector<unsigned int> m_IndicesData{};

	void SetPrimitive(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices)
	{
		for (unsigned int i = 0; i < NrOfVertices; i++)
		{
			m_VertexData.push_back(Vertices[i]);
		}

		for (unsigned int i = 0; i < NrOfIndices; i++)
		{
			m_IndicesData.push_back(Indices[i]);
		}
	}

public:
	Primitives() {};
	virtual ~Primitives() {};
	
	inline const Vertex* GetVertices() const { return m_VertexData.data(); }
	inline const unsigned int GetVerticesNumber() const { return m_VertexData.size(); }
	inline const unsigned int* GetIndices() const { return m_IndicesData.data(); }
	inline const unsigned int GetIndicesNumber() const { return m_IndicesData.size(); }
};

class Quad : public Primitives
{
public:
	Quad()
		: Primitives()
	{
		Vertex PlaneVertices[] = {
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
		};

		unsigned int ebo_plane_indices[] = { 0, 1, 2, 2, 3, 0 };

		SetPrimitive(
			PlaneVertices, sizeof(PlaneVertices) / sizeof(Vertex),
			ebo_plane_indices, sizeof(ebo_plane_indices) / sizeof(unsigned int));
	}

	~Quad() override {};
};