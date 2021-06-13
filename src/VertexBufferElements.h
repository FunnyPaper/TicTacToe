#pragma once

#include "GLerror.h"
#include <vector>

struct VertexBufferElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

//Do VAO cpp
template<typename T>
struct Identity { typedef T type; };

//Do VAO cpp
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElements> m_Elements;
	unsigned int m_Stride;

	template<typename T>
	void Push(unsigned int count, Identity<T>&&)
	{
		ASSERT(false);
	}
	void Push(unsigned int count, Identity<int>&&)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElements::GetSizeOfType(GL_UNSIGNED_INT);
	}
	void Push(unsigned int count, Identity<float>&&)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElements::GetSizeOfType(GL_FLOAT);
	}
	void Push(unsigned int count, Identity<unsigned char>&&)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElements::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
public:
	VertexBufferLayout() : m_Stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{
		Push(count, Identity<T>());
	}
	inline const std::vector<VertexBufferElements>& GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };
};