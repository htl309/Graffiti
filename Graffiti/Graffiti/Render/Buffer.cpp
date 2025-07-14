#include"gfpch.h"

#include"Buffer.h"

#include"Renderer.h"
#include"Graffiti/Graphic/OpenGL/OpenGLBuffer.h"
#include"Graffiti/Graphic/Vulkan/VulkanBuffer.h"


namespace Graffiti {
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(std::vector<Vertex>& vertices, uint32_t count)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLVertexBuffer>(vertices, count);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanVertexBuffer>(vertices, count);
		}

		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(std::vector<uint32_t>&  indices, uint32_t count)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanIndexBuffer>(indices, count);;
		}

		return nullptr;
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t unitsize, uint32_t count, uint32_t set, uint32_t binding)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLUniformBuffer>(unitsize, count,binding);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanUniformBuffer>(unitsize, count, set, binding);
		}

		return nullptr;
	}

	std::shared_ptr<StorageBuffer> StorageBuffer::Create(uint32_t unitsize, uint32_t count, uint32_t set, uint32_t binding)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLStorageBuffer>(unitsize, count, binding);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanStorageBuffer>(unitsize, count, set, binding);
		}

		return nullptr;
	}
}