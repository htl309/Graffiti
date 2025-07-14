#pragma once
#include<memory>
#include"Graffiti/Render/Buffer.h"
#include"Graffiti/Scene/Model.h"

namespace Graffiti {
	class VertexArray {
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual const std::shared_ptr<VertexBuffer>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        void AddModel(std::shared_ptr<Model> m_Model);
		static std::shared_ptr<VertexArray> Create();

        std::shared_ptr<Model> m_Model;
	};
}