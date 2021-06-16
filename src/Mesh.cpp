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
	// Konstruktor mesh
	// Zapisz transformacje do zmiennych
	m_Position = Position;
	m_Rotation = Rotation;
	m_Scale = Scale;
	// Stwórz VertexArrayObject
	// W wielkim skrócie VAO to obiekt OpenGL przechowuj¹cy dane odnoœnie po³o¿enia obiektu, kolorów i tekstur
	// Dodatkowo mo¿e przyjmowaæ inne informacje wymagane przy zastosowanym shaderze
	InitVAO(Vertices, NrOfVertices, Indices, NrOfIndices);
	// Zaktualizuj macierz modelu (ustawienie obiektu w pzestrzeni 3D)
	UpdateModelMatrix();
}

Mesh::Mesh(Primitives* primitive,
	const glm::vec3& Position, const glm::vec3& Rotation, const glm::vec3& Scale) : _Temporary_Handle(primitive)
{
	// Konstruktor mesh
	// Zapisz transformacje do zmiennych
	m_Position = Position;
	m_Rotation = Rotation;
	m_Scale = Scale;
	// Uzyskaj dostêp do iloœci wierzcho³ków i ich powtórzeñ
	m_NrOfIndices = static_cast<unsigned int>(primitive->GetIndicesNumber());
	m_NrOfVertices = static_cast<unsigned int>(primitive->GetVerticesNumber());
	// Stwórz VertexArrayObject
	// W wielkim skrócie VAO to obiekt OpenGL przechowuj¹cy dane odnoœnie po³o¿enia obiektu, kolorów i tekstur
	// Dodatkowo mo¿e przyjmowaæ inne informacje wymagane przy zastosowanym shaderze
	InitVAO(primitive);
	// Zaktualizuj macierz modelu (ustawienie obiektu w pzestrzeni 3D)
	UpdateModelMatrix();
}

Mesh::~Mesh()
{
	// Usuñ obiekty OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Usuñ wskaŸnik na primitive
	delete _Temporary_Handle;
}

void Mesh::InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices)
{
	// Stwórz VertexArrayObject
	// G³ówny obiekt do komunikacji z OpenGL
	glGenVertexArrays(1, &VAO);
	// Aktywuj VAO
	glBindVertexArray(VAO);
	// Stwórz VertexBufferObject
	// Przechowuje dane odnoœnie obiektu
	glGenBuffers(1, &VBO);
	// Aktywuj VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Wype³nij aktywne VBO danymi
	glBufferData(GL_ARRAY_BUFFER, NrOfVertices * sizeof(Vertex), Vertices, GL_STATIC_DRAW);
	// Stwórz ElementBufferObject
	// Przechowuje informacje o rozmieszczeniu wierzcho³ków
	// Dziêki temu obiektowi mo¿emy wyrenderowaæ kwadrat z czterech wierzcho³ków, a nie z szeœciu
	glGenBuffers(1, &EBO);
	// Aktywuj EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Wype³nij aktywne EBO danymi
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NrOfIndices * sizeof(unsigned int), Indices, GL_STATIC_DRAW);

	// Wybierz odpowiedni layout
	// Wewn¹trz plików .shader ka¿da zmienna opisana jest za pomoc¹ layoutu (numerek) - jest to tzw lokalizacja zmiennej
	// Mo¿emy wybraæ zmienn¹ na podstawie przypisanego layoutu - jeœli zmienna nie ma przypisanego layoutu to GLSL przypisuje go automatycznie
	// (oznacza to, ¿e taka zmienna ma przypisan¹ losow¹ wartoœæ po której nie da siê wprost okreœliæ czy to ta zmienna) -
	// wtedy konieczne jest u¿ywanie innej funkcji OpenGL (zastosowana wewn¹trz klasy Shader -> glUniformLocation)

	// Wymienione ni¿ej zosta³o makro offsetof które dla argumentów s, m zwraca (size_t)&(((s*)0)->m)
	// Mo¿na stosowaæ zamiennie lecz jest mniej czytelne -> chodzi o zwrócenie rozmiaru wskazanej zmiennej sk³adowej 
	// dla poprawnego rozlokowania danych w funkcji OpenGL

	// Aktywuj cztery layouty i wype³nij ka¿dy z nich danymi pozyskanymi z klasy Primitives
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Dezaktywuj VAO
	// W przeciwnym wypadku mo¿emy zepsuæ dzia³anie programu
	glBindVertexArray(0);
}
void Mesh::InitVAO(Primitives* primitive)
{
	// Stwórz VertexArrayObject
	// G³ówny obiekt do komunikacji z OpenGL
	glGenVertexArrays(1, &VAO);
	// Aktywuj VAO
	glBindVertexArray(VAO);
	// Stwórz VertexBufferObject
	// Przechowuje dane odnoœnie obiektu
	glGenBuffers(1, &VBO);
	// Aktywuj VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Wype³nij aktywne VBO danymi
	glBufferData(GL_ARRAY_BUFFER, m_NrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW);
	// Stwórz ElementBufferObject
	// Przechowuje informacje o rozmieszczeniu wierzcho³ków
	// Dziêki temu obiektowi mo¿emy wyrenderowaæ kwadrat z czterech wierzcho³ków, a nie z szeœciu
	glGenBuffers(1, &EBO);
	// Aktywuj EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Wype³nij aktywne EBO danymi
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NrOfIndices * sizeof(unsigned int), primitive->GetIndices(), GL_STATIC_DRAW);

	// Wybierz odpowiedni layout
	// Wewn¹trz plików .shader ka¿da zmienna opisana jest za pomoc¹ layoutu (numerek) - jest to tzw lokalizacja zmiennej
	// Mo¿emy wybraæ zmienn¹ na podstawie przypisanego layoutu - jeœli zmienna nie ma przypisanego layoutu to GLSL przypisuje go automatycznie
	// (oznacza to, ¿e taka zmienna ma przypisan¹ losow¹ wartoœæ po której nie da siê wprost okreœliæ czy to ta zmienna) -
	// wtedy konieczne jest u¿ywanie innej funkcji OpenGL (zastosowana wewn¹trz klasy Shader -> glUniformLocation)
	
	// Wymienione ni¿ej zosta³o makro offsetof które dla argumentów s, m zwraca (size_t)&(((s*)0)->m)
	// Mo¿na stosowaæ zamiennie lecz jest mniej czytelne -> chodzi o zwrócenie rozmiaru wskazanej zmiennej sk³adowej 
	// dla poprawnego rozlokowania danych w funkcji OpenGL
	
	// Aktywuj cztery layouty i wype³nij ka¿dy z nich danymi pozyskanymi z klasy Primitives
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Dezaktywuj VAO
	// W przeciwnym wypadku mo¿emy zepsuæ dzia³anie programu
	glBindVertexArray(0);
}

void Mesh::UpdateModelMatrix()
{
	// Zaktualizuj macierz modelu
	m_ModelMatrix = glm::mat4(1.0f);
	// Na transformacje obiektu sk³adaj¹ siê przesuniêcie, rotacja, skalowanie
	// Normalnie tak¹ macierz uzyskamy przez mno¿enie macierzy skalowania, rotacji, przesuniêcia (kolejnoœæ ma znaczenie)
	// GLM wykorzystuje odwrócony model macierzy, kompatybilny z OpenGL, dlatego te¿ operacje wykonujemy na odwrót
	// Przesuniêcie
	m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
	// Rotacja -> w oparciu o osie OX, OY, OZ
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Skalowanie
	m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}

void Mesh::UpdateUniforms(Shader* shader)
{
	// Zaktualizuj uniformy
	// Model -> transformacje obiektu
	shader->SetUniformMat4f("model", glm::value_ptr(m_ModelMatrix));
	// Model Normals -> transformacje wektorów u¿ywanych przy kalkulacji œwiat³a
	shader->SetUniformMat3f("model_normals", glm::value_ptr(GetNormalModelMatrix()));
}

void Mesh::Render(Shader* shader)
{
	// Rysuj obiekt
	// Zaktualizuj macierz modelu
	UpdateModelMatrix();
	// Zaktualizuj uniformy
	UpdateUniforms(shader);
	// Aktywuj VAO
	glBindVertexArray(VAO);
	// Jeœli posiadam dostêp do tablicy obrazuj¹cej wykorzystanie wierzcho³ków to rysuj za ich pomoc¹
	if (m_NrOfIndices)
		glDrawElements(GL_TRIANGLES, m_NrOfIndices, GL_UNSIGNED_INT, 0);
	// W przeciwnym wypadku rysuj za pomoc¹ indywidualnych wierzcho³ków ka¿dego trójk¹ta
	else glDrawArrays(GL_TRIANGLES, 0, m_NrOfVertices);
}