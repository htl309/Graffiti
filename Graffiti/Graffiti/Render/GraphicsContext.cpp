#include "gfpch.h"
#include "GraphicsContext.h"
#include "Graffiti/Render/Renderer.h"

#include "Graffiti/Graphic/OpenGL/OpenGLContext.h"
#include "Graffiti/Graphic/Vulkan/VulkanContext.h"
namespace Graffiti {
	std::vector< std::shared_ptr<GraphicsContext>> GraphicsContext::G_Context;

	std::string GraphicsContext::PhysicalGPU_Name;
	std::string GraphicsContext::GraphicAPI_Version;

	std::shared_ptr<GraphicsContext> GraphicsContext::Create(std::unique_ptr<Window>  window)
	{
		switch (Render::GetRenderAPI())
		{
		case RenderAPI::API::OpenGL:  return std::make_shared<OpenGLContext>(std::move(window));
		case RenderAPI::API::Vulkan:  return  std::make_shared<VulkanContext>(std::move(window));
		}

		return nullptr;
	}
}