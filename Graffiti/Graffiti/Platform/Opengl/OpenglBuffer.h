#pragma once
#include"Graffiti/Render/Buffer.h"
namespace Graffiti {

	class OpenglVertexBuffer:public VertexBuffer
	{
	public:
		OpenglVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenglVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout()const override;

	private:
		uint32_t m_RenderID;
		BufferLayout m_BufferLayout;
	};

	class OpenglIndexBuffer :public IndexBuffer
	{
	public:
		OpenglIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenglIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RenderID;

		uint32_t m_Count;
	};
}


