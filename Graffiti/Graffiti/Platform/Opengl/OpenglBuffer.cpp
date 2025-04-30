#include "gfpch.h"
#include "OpenglBuffer.h"
#include <glad/glad.h>
namespace Graffiti {



	OpenglVertexBuffer::OpenglVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); 
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenglVertexBuffer::~OpenglVertexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	}

	void OpenglVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void OpenglVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenglVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_BufferLayout = layout;
	}
	const BufferLayout& OpenglVertexBuffer::GetLayout() const
	{
		return m_BufferLayout;
	}
	/// <summary>
	/// IndexBuffer//////////////////////////////////IndexBuffer/////////////////////////////////
	/// </summary>
	
	OpenglIndexBuffer::OpenglIndexBuffer(uint32_t* indices, uint32_t count):m_Count(count)
	{
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenglIndexBuffer::~OpenglIndexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID); 
	}

	void OpenglIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}

	void OpenglIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}