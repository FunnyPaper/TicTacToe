#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_count;
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; }
};