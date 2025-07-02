#pragma once
#include "Graffiti/Render/Shader.h"
#include "Graffiti/Graphic/Vulkan/VulkanRenderAPI.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
namespace Graffiti {
	class VulkanShader :public Shader {
	public:
		VulkanShader(const std::string& shadername);
		VulkanShader(const std::string& shadername, std::initializer_list<std::pair<ShaderType, const std::string>> list);

		~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Load(ShaderType shadertype, const std::string& filepath)  override;

        virtual void AddPipeline(std::shared_ptr<PipelineConfigInfo> configInfo) override;

		//Link：Vulkan的管线创建是在这里进行，
		//等所有的Shader都被加载完毕，那么就可以绑定到管线上了
		//这里就创建管线，创建的管线真身放在VulkanRenderAPI中，这里就保留一个名字
        //注意！！！！！！这里创建的是普通的管线
		virtual void Link() override;

		virtual void UploadSceneData() override;
		virtual void SetSceneData(const SceneData& scenedata) override;
		virtual void UploadTransform() override;
		virtual void SetTransform(const glm::mat4& transform) override;


		virtual void SetUniformBuffer(const std::string& name, const void* value) override;
		virtual void UploadUniformBuffer(const std::string& name, uint32_t size, uint32_t count ,uint32_t set, uint32_t binding ) override;
		
		virtual void UploadTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding) override;
        virtual void SetTexture(std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding) override;

	private:
		std::vector<char> readFile(const std::string& filepath);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

	private:
		
		std::string m_Name;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		std::vector<VkShaderModule> m_ShaderModule;
		VulkanRenderAPI* m_RenderAPI;

        std::shared_ptr<VulkanPipelineLayout> m_PipelineLayout;
		std::vector<std::shared_ptr<PipelineConfigInfo>> m_PipelineConfigInfos;
	};
	
}


