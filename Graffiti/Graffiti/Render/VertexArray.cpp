#include"gfpch.h"
#include"VertexArray.h"
#include"RenderAPI.h"

#include"Graffiti/Platform/Opengl/OpenglVertexArray.h"
namespace Graffiti {

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		if (Render::GetRenderAPI() == RenderAPI::Opengl) {
			return std::make_shared<OpenglVertexArray>();
		}
		else if (Render::GetRenderAPI() == RenderAPI::Vulkan) {
			return nullptr;
		}

		return nullptr;
	}
}