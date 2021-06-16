#include "Texture.h"
#include "GLerror.h"
// Zewnêtrzna biblioteka u³atwiaj¹ca operacje na zdjêciach (odczyt i zapis do zmiennej)
#include "SOIL2.h"
#include <iostream>

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Filepath(filepath)
{
	// Konstruktor tekstury
	// Wykorzystanie funkcji ³aduj¹cej
	// W przeciwnym wypadku nale¿a³oby u¿yæ kilku funkcji ustawiaj¹cych (jak np wewn¹trz klasy GLFWinit zastosowano glfwWindowHint)
	// Oprócz tego w dalszym ci¹gu wymagane by by³o napisanie funkcji wczytuj¹cej dowolne zdjêcie
	// Du¿o wygodniejszym by³oby u¿ycie gotowej biblioteki, która dodatkowo jest kompatybilna z OpenGL -> bezpoœrednio tworzy obiekt OpenGL
	m_RendererID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}
// Konstruktor tekstury
Texture::Texture() : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr) {}
Texture::~Texture()
{
	// Destruktor tekstury
	// Usuñ obiekt OpenGL
	GLCall(glDeleteTextures(1, &m_RendererID));
}
void Texture::Bind(unsigned int slot) const
{
	// Aktywuj podany slot na teksturê i przypisz tam przechowywan¹ teksturê
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind()
{
	// Dezaktywuj teksturê w aktywnym slocie
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}