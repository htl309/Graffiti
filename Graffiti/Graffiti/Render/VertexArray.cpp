#include"gfpch.h"
#include"VertexArray.h"
#include"Renderer.h"

#include"Graffiti/Graphic/OpenGL/OpenGLVertexArray.h"
#include"Graffiti/Graphic/Vulkan/VulkanVertexArray.h"
namespace Graffiti {
    void VertexArray::AddModel(std::shared_ptr<Model> mode)
    {
        m_Model = mode;
       AddVertexBuffer(VertexBuffer::Create(m_Model->m_Vertices, m_Model->m_Vertices.size())); 
       AddIndexBuffer(IndexBuffer::Create(m_Model->m_Indices, m_Model->m_Indices.size())); 
    }
    std::shared_ptr<VertexArray> VertexArray::Create()
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLVertexArray>();
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanVertexArray>();
		}

		return nullptr;
	}
}