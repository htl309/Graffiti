#pragma once
#include"Graffiti/Render/Buffer.h"
#include "Graffiti/Scene/Vertex.h"

namespace Graffiti {

	class OpenGLVertexBuffer:public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(std::vector<Vertex>& vertices, uint32_t count);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual const BufferLayout& GetLayout()const override;

	private:
		uint32_t m_RenderID;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::vector<uint32_t>& indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RenderID;

		uint32_t m_Count;
	};

	class OpenGLUniformBuffer : public UniformBuffer{
		public:
			OpenGLUniformBuffer(uint32_t size ,uint32_t count, uint32_t binding);
			virtual ~OpenGLUniformBuffer();

			virtual void SetData(const void* data, uint32_t size = 0, uint32_t offset = 0) override;
		private:
			uint32_t m_RenderID = 0;
	};
	class OpenGLStorageBuffer : public StorageBuffer {
	public:
		OpenGLStorageBuffer(uint32_t size, uint32_t count, uint32_t binding);
		virtual ~OpenGLStorageBuffer();

		virtual void SetData(const void* data, uint32_t size = 0, uint32_t offset = 0) override;
	private:
		uint32_t m_RenderID = 0;
	};
}


