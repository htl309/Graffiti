#include"gfpch.h"

#include"Buffer.h"

#include"RenderAPI.h"
#include"Graffiti/Platform/Opengl/OpenglBuffer.h"
namespace Graffiti {





	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		if (Render::GetRenderAPI() == RenderAPI::Opengl) {
			return std::make_shared<OpenglVertexBuffer>(vertices,size);
		}
		else if (Render::GetRenderAPI() == RenderAPI::Vulkan) {
			return nullptr;
		}
	
		return nullptr;
	}



	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		if (Render::GetRenderAPI() == RenderAPI::Opengl) {
			return std::make_shared<OpenglIndexBuffer>(indices, count);
		}
		else if (Render::GetRenderAPI() == RenderAPI::Vulkan) {
			return nullptr;
		}

		return nullptr;
	}

}