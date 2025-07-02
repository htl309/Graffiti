#include "gfpch.h"
#include "Shader.h"
#include"Renderer.h"
#include"Graffiti/Graphic/OpenGL/OpenGLShader.h"
#include"Graffiti/Graphic/Vulkan/VulkanShader.h"
namespace Graffiti {

	
	std::shared_ptr<Shader> Shader::Create(const std::string& shadername)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLShader>(shadername);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanShader>(shadername);
		}
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& shadername,std::initializer_list<std::pair<ShaderType, const std::string>> list)
	{
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return std::make_shared<OpenGLShader>(shadername, list);
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return std::make_shared<VulkanShader>(shadername, list);
		}
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader> shader)
	{
		if(!Exists(name))
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Load(const std::string& name,ShaderType& shadertype, const std::string& filepath)
	{
		m_Shaders[name]->Load(shadertype, filepath);
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}