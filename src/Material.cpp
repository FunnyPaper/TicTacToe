#include "Material.h"
#include <iostream>
#include <string>

Material::Material(const Texture& ambient_texture, const Texture& diffuse_texture, const Texture& specular_texture, float shininess)
	: m_shininess(shininess), m_hasColor(false), m_hasTexture(true)
{
	m_texture[0] = ambient_texture;
	m_texture[1] = diffuse_texture;
	m_texture[2] = specular_texture;
}

Material::Material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, float shininess)
	: m_shininess(shininess), m_hasColor(true), m_hasTexture(false)
{
	m_colors[0] = ambient_color;
	m_colors[1] = diffuse_color;
	m_colors[2] = specular_color;
}

void Material::SendToShader(Shader& shader)
{
	if(m_hasColor)
	{
		shader.SetUniformVec3f("material.ambient_color", glm::value_ptr(m_colors[0]));
		shader.SetUniformVec3f("material.diffuse_color", glm::value_ptr(m_colors[1]));
		shader.SetUniformVec3f("material.specular_color", glm::value_ptr(m_colors[2]));
		shader.SetUniform1i("is_texture_attached", 0);
	}
	if(m_hasTexture)
	{
		//shader.SetUniform1i("material.ambient_texture" + std::to_string(m_texture[0].GetTextureUnit()), m_texture[0].GetTextureUnit());
		//shader.SetUniform1i("material.diffuse_texture" + std::to_string(m_texture[1].GetTextureUnit()), m_texture[1].GetTextureUnit());
		//shader.SetUniform1i("material.specular_texture" + std::to_string(m_texture[2].GetTextureUnit()), m_texture[2].GetTextureUnit());
		//m_texture[0].Bind(0); 
		m_texture[1].Bind(0);
		m_texture[2].Bind(1);
		shader.SetUniform1i("is_texture_attached", 1);
	}
	shader.SetUniform1f("material.shininess", m_shininess);
}