#pragma once
#include"Graffiti/Render/VertexArray.h"
namespace Graffiti {

	class OpenglVertexArray: public VertexArray
	{
	public:
		OpenglVertexArray();
		virtual ~OpenglVertexArray();
		virtual void Bind() const;
		virtual void Unbind() const;


		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer()  const override  { return m_IndexBuffer; }
	private:
		uint32_t m_RenderID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}


