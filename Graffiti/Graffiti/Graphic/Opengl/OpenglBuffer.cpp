#include "gfpch.h"
#include "OpenGLBuffer.h"


#include <glad/glad.h>
namespace Graffiti {



	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<Vertex>& vertices, uint32_t count)
	{
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); 
		glBufferData(GL_ARRAY_BUFFER, count* sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		m_BufferLayout = {
				{ ShaderDataType::Float3,"inposition"},
				{ ShaderDataType::Float3,"innormal"},
				{ ShaderDataType::Float2,"intexCord" }
		};
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_BufferLayout = layout;
	}
	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_BufferLayout;
	}
	/// <summary>
	/// IndexBuffer//////////////////////////////////IndexBuffer/////////////////////////////////
	/// </summary>
	
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>&  indices, uint32_t count):m_Count(count)
	{
		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID); 
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t count, uint32_t binding)
	{
		m_Set = size*count;
		m_Binding = binding;

		glCreateBuffers(1, &m_RenderID);
		glNamedBufferData(m_RenderID, m_Set, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RenderID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RenderID, offset, size, data);
	}

}