#pragma once

#include <string>

class Texture
{
	// Klasa do odczytu i przechowywania tekstur
private:
	// Unikatowe ID tekstury
	unsigned int m_RendererID;
	// �cie�ka dost�pu do tekstury
	std::string m_Filepath{};
	// Podstawowe dane odno�nie tekstury - szeroko��, d�ugo��, g��bia
	int m_Width, m_Height, m_BPP;
	// Wska�nik przechowywuj�cy wczytaj� tekstur� (prawdopodobnie odczyt binarny)
	unsigned char* m_Buffer;

public:
	// Konstruktor tekstury
	Texture(const std::string& filepath);
	// Konstruktor tekstury
	Texture();
	// Destruktor tekstury
	~Texture();
	// Aktywuj tekstur�
	void Bind(unsigned int slot) const;
	// Dezaktywuj aktywn� tekstur�
	static void Unbind();
	// Zwr�� szeroko��/ d�ugo�� zdj�cia
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

