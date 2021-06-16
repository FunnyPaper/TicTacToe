#include "Texture.h"
#include "GLerror.h"
// Zewn�trzna biblioteka u�atwiaj�ca operacje na zdj�ciach (odczyt i zapis do zmiennej)
#include "SOIL2.h"
#include <iostream>

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Filepath(filepath)
{
	// Konstruktor tekstury
	// Wykorzystanie funkcji �aduj�cej
	// W przeciwnym wypadku nale�a�oby u�y� kilku funkcji ustawiaj�cych (jak np wewn�trz klasy GLFWinit zastosowano glfwWindowHint)
	// Opr�cz tego w dalszym ci�gu wymagane by by�o napisanie funkcji wczytuj�cej dowolne zdj�cie
	// Du�o wygodniejszym by�oby u�ycie gotowej biblioteki, kt�ra dodatkowo jest kompatybilna z OpenGL -> bezpo�rednio tworzy obiekt OpenGL
	m_RendererID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}
// Konstruktor tekstury
Texture::Texture() : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr) {}
Texture::~Texture()
{
	// Destruktor tekstury
	// Usu� obiekt OpenGL
	GLCall(glDeleteTextures(1, &m_RendererID));
}
void Texture::Bind(unsigned int slot) const
{
	// Aktywuj podany slot na tekstur� i przypisz tam przechowywan� tekstur�
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind()
{
	// Dezaktywuj tekstur� w aktywnym slocie
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}