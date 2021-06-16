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
	// Stw�rz VertexArrayObject
	// W wielkim skr�cie VAO to obiekt OpenGL przechowuj�cy dane odno�nie po�o�enia obiektu, kolor�w i tekstur
	// Dodatkowo mo�e przyjmowa� inne informacje wymagane przy zastosowanym shaderze
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
	// Uzyskaj dost�p do ilo�ci wierzcho�k�w i ich powt�rze�
	m_NrOfIndices = static_cast<unsigned int>(primitive->GetIndicesNumber());
	m_NrOfVertices = static_cast<unsigned int>(primitive->GetVerticesNumber());
	// Stw�rz VertexArrayObject
	// W wielkim skr�cie VAO to obiekt OpenGL przechowuj�cy dane odno�nie po�o�enia obiektu, kolor�w i tekstur
	// Dodatkowo mo�e przyjmowa� inne informacje wymagane przy zastosowanym shaderze
	InitVAO(primitive);
	// Zaktualizuj macierz modelu (ustawienie obiektu w pzestrzeni 3D)
	UpdateModelMatrix();
}

Mesh::~Mesh()
{
	// Usu� obiekty OpenGL
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Usu� wska�nik na primitive
	delete _Temporary_Handle;
}

void Mesh::InitVAO(Vertex* Vertices, unsigned int NrOfVertices, unsigned int* Indices, unsigned int NrOfIndices)
{
	// Stw�rz VertexArrayObject
	// G��wny obiekt do komunikacji z OpenGL
	glGenVertexArrays(1, &VAO);
	// Aktywuj VAO
	glBindVertexArray(VAO);
	// Stw�rz VertexBufferObject
	// Przechowuje dane odno�nie obiektu
	glGenBuffers(1, &VBO);
	// Aktywuj VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Wype�nij aktywne VBO danymi
	glBufferData(GL_ARRAY_BUFFER, NrOfVertices * sizeof(Vertex), Vertices, GL_STATIC_DRAW);
	// Stw�rz ElementBufferObject
	// Przechowuje informacje o rozmieszczeniu wierzcho�k�w
	// Dzi�ki temu obiektowi mo�emy wyrenderowa� kwadrat z czterech wierzcho�k�w, a nie z sze�ciu
	glGenBuffers(1, &EBO);
	// Aktywuj EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Wype�nij aktywne EBO danymi
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NrOfIndices * sizeof(unsigned int), Indices, GL_STATIC_DRAW);

	// Wybierz odpowiedni layout
	// Wewn�trz plik�w .shader ka�da zmienna opisana jest za pomoc� layoutu (numerek) - jest to tzw lokalizacja zmiennej
	// Mo�emy wybra� zmienn� na podstawie przypisanego layoutu - je�li zmienna nie ma przypisanego layoutu to GLSL przypisuje go automatycznie
	// (oznacza to, �e taka zmienna ma przypisan� losow� warto�� po kt�rej nie da si� wprost okre�li� czy to ta zmienna) -
	// wtedy konieczne jest u�ywanie innej funkcji OpenGL (zastosowana wewn�trz klasy Shader -> glUniformLocation)

	// Wymienione ni�ej zosta�o makro offsetof kt�re dla argument�w s, m zwraca (size_t)&(((s*)0)->m)
	// Mo�na stosowa� zamiennie lecz jest mniej czytelne -> chodzi o zwr�cenie rozmiaru wskazanej zmiennej sk�adowej 
	// dla poprawnego rozlokowania danych w funkcji OpenGL

	// Aktywuj cztery layouty i wype�nij ka�dy z nich danymi pozyskanymi z klasy Primitives
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Dezaktywuj VAO
	// W przeciwnym wypadku mo�emy zepsu� dzia�anie programu
	glBindVertexArray(0);
}
void Mesh::InitVAO(Primitives* primitive)
{
	// Stw�rz VertexArrayObject
	// G��wny obiekt do komunikacji z OpenGL
	glGenVertexArrays(1, &VAO);
	// Aktywuj VAO
	glBindVertexArray(VAO);
	// Stw�rz VertexBufferObject
	// Przechowuje dane odno�nie obiektu
	glGenBuffers(1, &VBO);
	// Aktywuj VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Wype�nij aktywne VBO danymi
	glBufferData(GL_ARRAY_BUFFER, m_NrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW);
	// Stw�rz ElementBufferObject
	// Przechowuje informacje o rozmieszczeniu wierzcho�k�w
	// Dzi�ki temu obiektowi mo�emy wyrenderowa� kwadrat z czterech wierzcho�k�w, a nie z sze�ciu
	glGenBuffers(1, &EBO);
	// Aktywuj EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Wype�nij aktywne EBO danymi
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NrOfIndices * sizeof(unsigned int), primitive->GetIndices(), GL_STATIC_DRAW);

	// Wybierz odpowiedni layout
	// Wewn�trz plik�w .shader ka�da zmienna opisana jest za pomoc� layoutu (numerek) - jest to tzw lokalizacja zmiennej
	// Mo�emy wybra� zmienn� na podstawie przypisanego layoutu - je�li zmienna nie ma przypisanego layoutu to GLSL przypisuje go automatycznie
	// (oznacza to, �e taka zmienna ma przypisan� losow� warto�� po kt�rej nie da si� wprost okre�li� czy to ta zmienna) -
	// wtedy konieczne jest u�ywanie innej funkcji OpenGL (zastosowana wewn�trz klasy Shader -> glUniformLocation)
	
	// Wymienione ni�ej zosta�o makro offsetof kt�re dla argument�w s, m zwraca (size_t)&(((s*)0)->m)
	// Mo�na stosowa� zamiennie lecz jest mniej czytelne -> chodzi o zwr�cenie rozmiaru wskazanej zmiennej sk�adowej 
	// dla poprawnego rozlokowania danych w funkcji OpenGL
	
	// Aktywuj cztery layouty i wype�nij ka�dy z nich danymi pozyskanymi z klasy Primitives
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texturecoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// Dezaktywuj VAO
	// W przeciwnym wypadku mo�emy zepsu� dzia�anie programu
	glBindVertexArray(0);
}

void Mesh::UpdateModelMatrix()
{
	// Zaktualizuj macierz modelu
	m_ModelMatrix = glm::mat4(1.0f);
	// Na transformacje obiektu sk�adaj� si� przesuni�cie, rotacja, skalowanie
	// Normalnie tak� macierz uzyskamy przez mno�enie macierzy skalowania, rotacji, przesuni�cia (kolejno�� ma znaczenie)
	// GLM wykorzystuje odwr�cony model macierzy, kompatybilny z OpenGL, dlatego te� operacje wykonujemy na odwr�t
	// Przesuni�cie
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
	// Model Normals -> transformacje wektor�w u�ywanych przy kalkulacji �wiat�a
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
	// Je�li posiadam dost�p do tablicy obrazuj�cej wykorzystanie wierzcho�k�w to rysuj za ich pomoc�
	if (m_NrOfIndices)
		glDrawElements(GL_TRIANGLES, m_NrOfIndices, GL_UNSIGNED_INT, 0);
	// W przeciwnym wypadku rysuj za pomoc� indywidualnych wierzcho�k�w ka�dego tr�jk�ta
	else glDrawArrays(GL_TRIANGLES, 0, m_NrOfVertices);
}