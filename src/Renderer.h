#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

enum class MODE
{
	NONE, ARRAY_DRAW, INDEX_ARRAY_DRAW 
};

class Renderer
{
private:
	IndexBuffer m_ebo;
	VertexArray m_vao;
	VertexBuffer m_vbo;
	Shader m_shader;
public:
	Renderer(const VertexArray& vao, const VertexBuffer& vbo, const IndexBuffer& ebo, const Shader& shader);

	void BindObjectsForArrayDraw() const;
	void BindObjectsForIndexDraw() const;
	void UnbindObjects() const;
	static void ClearScreen(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	void DrawArray(unsigned int pos = 0) const;
	void DrawArrayByIndex() const;

	inline void ChangeCurrentVAO(const VertexArray& vao) { m_vao = vao; }
	inline void ChangeCurrentVBO(const VertexBuffer& vbo) { m_vbo = vbo; }
	inline void ChangeCurrentEBO(const IndexBuffer& ebo) { m_ebo = ebo; }
	inline void ChangeCurrentShader(const Shader& shader) { m_shader = shader; }

	inline IndexBuffer* GetMyCurrentEBO() { return &m_ebo; }
	inline VertexArray* GetMyCurrentVAO() { return &m_vao; }
	inline VertexBuffer* GetMyCurrentVBO() { return &m_vbo; }
	inline Shader* GetMyCurrentShader() { return &m_shader; }
};

