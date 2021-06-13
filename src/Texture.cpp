#include "Texture.h"
#include "GLerror.h"
#include "SOIL2.h"
#include <iostream>

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Filepath(filepath)
{
	/*m_Buffer = SOIL_load_image(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));*/
	m_RendererID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	//if (m_Buffer)
	//	SOIL_free_image_data(m_Buffer);
	//else std::cout << "Creation Failed" << std::endl;
}
Texture::Texture() : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr) {}
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
	
}
void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}