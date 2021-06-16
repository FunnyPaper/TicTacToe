#pragma once
#include "Vertex.h"
#include <vector>
#include <glm\glm.hpp>

class Primitives
{
	// Klasa bazowa przechowuj¹ca wstêpne ustawienia obiektów (pozycja wierzcho³ków, kolory, rozstawienie tekstur, etc)
protected:
	// Vectory przechowuj¹ce dane o wierzcho³kach i ich uzyciu
	std::vector<Vertex> m_VertexData{};
	std::vector<unsigned int> m_IndicesData{};

	// Wype³nij przechowywane wektory danymi wstêpnymi
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
	// Konstruktor primitives
	Primitives() {};
	// Wirtualny destruktor primitives
	// Mo¿liwoœæ poprawnego usuwania obiektów tej klasy jeœli zastosowano polimorfiê
	virtual ~Primitives() {};
	// Metody zwracaj¹ce informacje o przechowywanych danych
	inline const Vertex* GetVertices() const { return m_VertexData.data(); }
	inline const size_t GetVerticesNumber() const { return m_VertexData.size(); }
	inline const unsigned int* GetIndices() const { return m_IndicesData.data(); }
	inline const size_t GetIndicesNumber() const { return m_IndicesData.size(); }
};

class Quad : public Primitives
{
	// Klasa kwadratu
public:
	Quad() : Primitives()
	{
		// Konstruktor Quad
		// Stwórz tablicê dla kwadratu z wstêpnymi danymi
		Vertex PlaneVertices[] = {
		glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
		};
		// Stwórrz tablicê z informacj¹ o wykorzystaniu wierzcho³ków
		unsigned int ebo_plane_indices[] = { 0, 1, 2, 2, 3, 0 };
		// Ustaw dane zapisane w konstruktorze jako domyœlne
		SetPrimitive(
			PlaneVertices, sizeof(PlaneVertices) / sizeof(Vertex),
			ebo_plane_indices, sizeof(ebo_plane_indices) / sizeof(unsigned int)
		);
	}
	// Destruktor Quad
	~Quad() override {};
};