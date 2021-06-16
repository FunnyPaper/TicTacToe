#pragma once
#include "Vertex.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Primitives.h"

class Mesh
{
	// Klasa odpowiedzialna za transformacje i rysowanie obiektu
private:
	// Przechowuj pozycj�, rotacj� i skal� obiektu
	glm::vec3 m_Position{};
	glm::vec3 m_Rotation{};
	glm::vec3 m_Scale{};
	// Przechowuj macierz modelu (to gdze wzgl�dem pocz�tku uk�adu ma znajdowa� si� obiekt)
	glm::mat4 m_ModelMatrix{};
	// Unikatowe ID dla obiekt�w OpenGL
	unsigned int VAO{}, VBO{}, EBO{};
	// Przechowuj ilo�� wierzcho�k�w (dla ka�dej �ciany oraz unikatow�)
	// OpenGL renderuje obiekty w postaci po��czonych tr�jk�t�w i np �eby wyrenderowa� kwadrat potrzeba dwa tr�jk�ty
	// Ka�dy z tr�jk�t�w ma po trzy wierzcho�ki wi�c kwadrat b�dzie mia� ich sze�� (opr�cz tego zapisujemy standardow� ilo�� wierzcho�k�w dla
	// tej figury czyli cztery)
	unsigned int m_NrOfIndices{}, m_NrOfVertices{};
	// Wska�nik tymczasowy na warto�ci prymitywne (tylko po to by wewn�trz konstruktora mo�na by�o wpisa� new Primitives())
	Primitives* _Temporary_Handle;

public:
	// Konstruktor mesh
	Mesh(Vertex* Vertices, 
		unsigned int NrOfVertices, 
		unsigned int* Indices = NULL, 
		unsigned int NrOfIndices = 0, 
		const glm::vec3& Position = glm::vec3(0.0f), 
		const glm::vec3& Rotation = glm::vec3(0.0f), 
		const glm::vec3& Scale = glm::vec3(1.0f));
	// Konstruktor mesh
	Mesh(Primitives* primitive,
		const glm::vec3& Position = glm::vec3(0.0f), 
		const glm::vec3& Rotation = glm::vec3(0.0f), 
		const glm::vec3& Scale = glm::vec3(1.0f));
	// Destruktor mesh
	~Mesh();
	// Rysuj obiekt
	void Render(Shader* shader);
	// Zmie� transformacj� obiektu
	// Zmie� ca�kowite odniesienie -> tak jakby dopiero co dokonano stworzenia obiektu
	inline void SetPosition(glm::vec3 position) { m_Position = position; }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }
	// Zmie� obiekt w oparciu o obecne dane
	inline void Move(glm::vec3 DirectionVector) { m_Position += DirectionVector; }
	inline void Rotate(glm::vec3 RotationVector) { m_Rotation += RotationVector; }
	inline void Scale(glm::vec3 ScaleVector) { m_Scale += ScaleVector; }

private:
	// Zainicjuj obiekty OpenGL
	void InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices);
	void InitVAO(Primitives* primitive);
	// Zaktualizuj macierz modelu
	void UpdateModelMatrix();
	// Uzyskaj znormalizowan� macierz modelu (staje si� istotne kiedy dochodzi manipulacja pozycj� kamery oraz symulacja �wiat�a)
	const glm::mat3 GetNormalModelMatrix() { return glm::mat3(glm::transpose(glm::inverse(m_ModelMatrix))); }
	// Zaktualizuj uniformy
	// Uniformy to zmienne wewn�trz vertex i fragment shader z kt�rymi mo�na nawi�za� kontakt i lekko zmodyfikowa� dzia�anie shadera
	void UpdateUniforms(Shader* shader);
};

