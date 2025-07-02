#pragma once
#include"gfpch.h"
#include"RenderCommand.h"

#include"Graffiti/Graphic/OpenGL/OpenGLRenderAPI.h"
#include"Graffiti/Graphic/Vulkan/VulkanRenderAPI.h"

#include "Graffiti/Graphic/OpenGL/OpenGLImGuiLayer.h"
#include "Graffiti/Graphic/Vulkan/VulkanImGuiLayer.h"
namespace Graffiti {

   
	RenderAPI* Graffiti::RenderCommand::s_RenderAPI;
   
	void RenderCommand::Create()
	{
		switch (RenderAPI::GetRenderAPI())
		{
		case RenderAPI::API::OpenGL: { RenderCommand::s_RenderAPI = new OpenGLRenderAPI; break; }
		case RenderAPI::API::Vulkan: { RenderCommand::s_RenderAPI = new VulkanRenderAPI; break; }

		};
	}
	RenderAPI* RenderCommand::GetRenderAPI()
	{
		return s_RenderAPI;
	}

	ImTextureID RenderCommand::GetTextureID(std::shared_ptr<Texture> Texture) {
		switch (RenderAPI::GetRenderAPI())
		{
		case RenderAPI::API::OpenGL: { return OpenGLImGuiLayer::GetTextureID(Texture); }
		case RenderAPI::API::Vulkan: { return VulkanImGuiLayer::GetTextureID(Texture); }

		};
	}

}