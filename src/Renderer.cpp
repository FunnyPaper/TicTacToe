#include "Renderer.h"
#include "GLerror.h"

Renderer::Renderer(const VertexArray& vao, const VertexBuffer& vbo, const IndexBuffer& ebo, const Shader& shader)
	: m_vao(vao), m_vbo(vbo), m_ebo(ebo), m_shader(shader) {}
void Renderer::ClearScreen(float r, float g, float b, float a)
{
	GLCall(glClearColor(r, g, b, a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
void Renderer::DrawArray(unsigned int pos) const
{
	BindObjectsForArrayDraw();
	GLCall(glDrawArrays(GL_TRIANGLES, pos, m_vbo.GetCount()));
}
void Renderer::DrawArrayByIndex() const
{
	BindObjectsForIndexDraw();
	GLCall(glDrawElements(GL_TRIANGLES, m_ebo.GetCount(), GL_UNSIGNED_INT, 0));
}
void Renderer::BindObjectsForArrayDraw() const
{
	m_vao.Bind();
	m_vbo.Bind();
	m_ebo.Unbind();
	m_shader.Bind();
}

void Renderer::BindObjectsForIndexDraw() const
{
	m_vao.Bind();
	m_vbo.Bind();
	m_ebo.Bind();
	m_shader.Bind();
}

void Renderer::UnbindObjects() const
{
	m_vbo.Unbind();
	m_ebo.Unbind();
	m_vao.Unbind();
	m_shader.Unbind();
}