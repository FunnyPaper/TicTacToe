#pragma once

#include <string>

class Texture
{
	// Klasa do odczytu i przechowywania tekstur
private:
	// Unikatowe ID tekstury
	unsigned int m_RendererID;
	// Œcie¿ka dostêpu do tekstury
	std::string m_Filepath{};
	// Podstawowe dane odnoœnie tekstury - szerokoœæ, d³ugoœæ, g³êbia
	int m_Width, m_Height, m_BPP;
	// WskaŸnik przechowywuj¹cy wczytaj¹ teksturê (prawdopodobnie odczyt binarny)
	unsigned char* m_Buffer;

public:
	// Konstruktor tekstury
	Texture(const std::string& filepath);
	// Konstruktor tekstury
	Texture();
	// Destruktor tekstury
	~Texture();
	// Aktywuj teksturê
	void Bind(unsigned int slot) const;
	// Dezaktywuj aktywn¹ teksturê
	static void Unbind();
	// Zwróæ szerokoœæ/ d³ugoœæ zdjêcia
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

