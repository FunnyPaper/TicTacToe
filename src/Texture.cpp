#include "Texture.h"
#include "GLerror.h"
#include "SOIL2.h"
#include <iostream>

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0), m_Buffer(nullptr), m_Filepath(filepath)
{
	m_RendererID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
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