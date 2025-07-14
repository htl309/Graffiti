#include"gfpch.h"

#include"VulkanVertexArray.h"
namespace Graffiti {
	VulkanVertexArray::VulkanVertexArray()
	{
	}
	VulkanVertexArray::~VulkanVertexArray()
	{
	}
	void VulkanVertexArray::Bind() const
	{
		m_VertexBuffers->Bind();
		m_IndexBuffer->Bind();
	}
	void VulkanVertexArray::Unbind() const
	{
	}
	void VulkanVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_VertexBuffers=vertexBuffer;
	}
	void VulkanVertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer=indexBuffer;
	}
	void VulkanVertexArray::bindPipeline()
	{
		
	}
}