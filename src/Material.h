#pragma once
#include "Texture.h"
#include <glm\glm.hpp>
#include "Shader.h"

class Material
{
public:
	Material(const Texture& ambient_texture, const Texture& diffuse_texture, const Texture& specular_texture, float shininess);
	Material(const glm::vec3& ambient_color, const glm::vec3& diffuse_color, const glm::vec3& specular_color, float shininess);
	const Texture* GetTextures() const { return m_texture; }
	const glm::vec3* GetColors() const { return m_colors; }

	void SendToShader(Shader& shader);

private:
	Texture m_texture[3]{};
	glm::vec3 m_colors[3]{};
	float m_shininess{};
	bool m_hasColor, m_hasTexture;
};