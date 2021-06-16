#pragma once
#include <glm\glm.hpp>

struct Vertex
{
	// Struktura przechowywująca informacje o wierzchołku
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texturecoord;
	glm::vec3 normal;
};

