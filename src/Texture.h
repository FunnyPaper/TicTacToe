#pragma once

#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath{};
	int m_Width, m_Height, m_BPP;
	unsigned char* m_Buffer;
public:
	Texture(const std::string& filepath);
	Texture();
	~Texture();

	void Bind(unsigned int slot) const;
	static void Unbind();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

