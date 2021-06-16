#pragma once
#include "Vertex.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Primitives.h"

class Mesh
{
	// Klasa odpowiedzialna za transformacje i rysowanie obiektu
private:
	// Przechowuj pozycjê, rotacjê i skalê obiektu
	glm::vec3 m_Position{};
	glm::vec3 m_Rotation{};
	glm::vec3 m_Scale{};
	// Przechowuj macierz modelu (to gdze wzglêdem pocz¹tku uk³adu ma znajdowaæ siê obiekt)
	glm::mat4 m_ModelMatrix{};
	// Unikatowe ID dla obiektów OpenGL
	unsigned int VAO{}, VBO{}, EBO{};
	// Przechowuj iloœæ wierzcho³ków (dla ka¿dej œciany oraz unikatow¹)
	// OpenGL renderuje obiekty w postaci po³¹czonych trójk¹tów i np ¿eby wyrenderowaæ kwadrat potrzeba dwa trójk¹ty
	// Ka¿dy z trójk¹tów ma po trzy wierzcho³ki wiêc kwadrat bêdzie mia³ ich szeœæ (oprócz tego zapisujemy standardow¹ iloœæ wierzcho³ków dla
	// tej figury czyli cztery)
	unsigned int m_NrOfIndices{}, m_NrOfVertices{};
	// WskaŸnik tymczasowy na wartoœci prymitywne (tylko po to by wewn¹trz konstruktora mo¿na by³o wpisaæ new Primitives())
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
	// Zmieñ transformacjê obiektu
	// Zmieñ ca³kowite odniesienie -> tak jakby dopiero co dokonano stworzenia obiektu
	inline void SetPosition(glm::vec3 position) { m_Position = position; }
	inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
	inline void SetScale(glm::vec3 scale) { m_Scale = scale; }
	// Zmieñ obiekt w oparciu o obecne dane
	inline void Move(glm::vec3 DirectionVector) { m_Position += DirectionVector; }
	inline void Rotate(glm::vec3 RotationVector) { m_Rotation += RotationVector; }
	inline void Scale(glm::vec3 ScaleVector) { m_Scale += ScaleVector; }

private:
	// Zainicjuj obiekty OpenGL
	void InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices);
	void InitVAO(Primitives* primitive);
	// Zaktualizuj macierz modelu
	void UpdateModelMatrix();
	// Uzyskaj znormalizowan¹ macierz modelu (staje siê istotne kiedy dochodzi manipulacja pozycj¹ kamery oraz symulacja œwiat³a)
	const glm::mat3 GetNormalModelMatrix() { return glm::mat3(glm::transpose(glm::inverse(m_ModelMatrix))); }
	// Zaktualizuj uniformy
	// Uniformy to zmienne wewn¹trz vertex i fragment shader z którymi mo¿na nawi¹zaæ kontakt i lekko zmodyfikowaæ dzia³anie shadera
	void UpdateUniforms(Shader* shader);
};

