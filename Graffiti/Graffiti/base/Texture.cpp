#include"gfpch.h"
#include"Texture.h"

#include"Graffiti/Render/Renderer.h"
#include"Graffiti/Graphic/OpenGL/OpenGLTexture.h"
#include"Graffiti/Graphic/Vulkan/VulkanTexture.h"

namespace Graffiti {
	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		
			if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
				return std::make_shared<OpenGLTexture>(path);
			}
			else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
				return std::make_shared<VulkanTexture>(path);
			}

			return nullptr;
		
	}

    // OpenGL 纹理创建函数（示例）
    std::shared_ptr<Texture> Texture::Create(const tinygltf::Image& image) {

        if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
            return std::make_shared<OpenGLTexture>(image);
        }
        else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
            return std::make_shared<VulkanTexture>(image);
        }

        return nullptr;
      
    }
}