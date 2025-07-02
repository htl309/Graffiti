#include "gfpch.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "Graffiti/Render/RenderCommand.h"
#include "Graffiti/Render/Renderer.h"
namespace Graffiti {
	VulkanShader::VulkanShader(const std::string& shadername)
		:m_Name(shadername)
	{
        m_RenderAPI = dynamic_cast<VulkanRenderAPI*>(RenderCommand::GetRenderAPI());
		m_RenderAPI->m_PipelineMap.insert({ m_Name,std::vector<VulkanPipeline>() });

		m_PipelineLayout = std::make_shared<VulkanPipelineLayout>();
		m_PipelineConfigInfos.push_back(std::make_shared<PipelineConfigInfo>(PipelineState::Common));

		UploadSceneData();
	}
	VulkanShader::VulkanShader(const std::string& shadername, std::initializer_list<std::pair<ShaderType, const std::string>> list)
		:m_Name(shadername)
	{
		m_RenderAPI = dynamic_cast<VulkanRenderAPI*>(RenderCommand::GetRenderAPI());
		m_RenderAPI->m_PipelineMap.insert({ m_Name,std::vector<VulkanPipeline>() });

		m_PipelineLayout = std::make_shared<VulkanPipelineLayout>();
		m_PipelineConfigInfos.push_back(std::make_shared<PipelineConfigInfo>(PipelineState::Common));

        UploadSceneData();

		for (auto& [Type, Path] : list) {
			Load(Type, Path);  
		}  
	}
	VulkanShader::~VulkanShader()
	{
	
	}
	void VulkanShader::Bind() const
	{
		m_RenderAPI->BindPipeline(m_Name);
	}
	void VulkanShader::Unbind() const
	{
	}
	void VulkanShader::Load(ShaderType shadertype, const std::string& filepath)
	{
		VkPipelineShaderStageCreateInfo shaderStagesInfo;

		shaderStagesInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		
		switch (shadertype)
		{
		case VertexShader:
		{
			shaderStagesInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		}
		case FragmentShader:
		{
			shaderStagesInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		}
		case GeometryShader:
		{
			shaderStagesInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
			break;
		}
		case TaskShader:
		{
			shaderStagesInfo.stage = VK_SHADER_STAGE_TASK_BIT_NV;
			break;
		}
		case MeshShader:
		{
			shaderStagesInfo.stage = VK_SHADER_STAGE_MESH_BIT_NV;
			break;
		}
		default:
			break;
		}

		VkShaderModule ShaderModule;
		m_ShaderModule.push_back(ShaderModule);
		auto ShaderCode = readFile("Shaders/Vulkan/" + filepath + ".spv");
		createShaderModule(ShaderCode, &m_ShaderModule[m_ShaderModule.size()-1]);
		
	
		shaderStagesInfo.module = m_ShaderModule[m_ShaderModule.size() - 1];
		shaderStagesInfo.pName = "main";
		shaderStagesInfo.flags = 0;
		shaderStagesInfo.pNext = nullptr;
		shaderStagesInfo.pSpecializationInfo = nullptr;

		
		m_ShaderStages.push_back(shaderStagesInfo);
	}
    void VulkanShader::AddPipeline(std::shared_ptr<PipelineConfigInfo> configInfo)
    {
		m_PipelineConfigInfos.push_back(configInfo);
    }
	void VulkanShader::Link()
	{
		auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);

		m_PipelineLayout->buildDescriptorSets(); 
		m_PipelineLayout->createPipelineLayout();	

        std::vector<VulkanPipeline>& pipelines = m_RenderAPI->m_PipelineMap[m_Name]; 
		
        //∞Û∂®µΩset=0,bing=0…œ
        for (int i = 0; i < m_PipelineConfigInfos.size(); i++)
        {
			m_PipelineConfigInfos[i]->m_VulkanPipelineLayout = m_PipelineLayout;
			m_PipelineConfigInfos[i]->renderPass = context->getSwapChainRenderPass();
			
            pipelines.push_back(VulkanPipeline(m_ShaderStages, m_PipelineConfigInfos[i]));
		
        }  
	}
	void VulkanShader::UploadSceneData()
	{
       m_PipelineLayout->UploadUniform("u_SceneData", sizeof(SceneData), 1, 0, 0);
	}
	void VulkanShader::SetSceneData(const SceneData& scenedata)
	{
		m_PipelineLayout->m_Data["u_SceneData"]->SetData(&scenedata);
	}
	void VulkanShader::UploadTransform()
	{
		
	}
	void VulkanShader::SetTransform(const glm::mat4& transform)
	{
        std::vector<VulkanPipeline>& pipelines = m_RenderAPI->m_PipelineMap[m_Name];
        for (int i = 0; i < pipelines.size(); i++)
        {
            pipelines[i].SetPushConstants(sizeof(glm::mat4), &transform);
        }
	}
	void VulkanShader::SetUniformBuffer(const std::string& name, const void* value)
	{
        m_PipelineLayout->m_Data[name]->SetData(value);    
	}
	void VulkanShader::UploadUniformBuffer(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding)
	{
        m_PipelineLayout->UploadUniform(name, size, count, set, binding);
	}

	void VulkanShader::UploadTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding)
	{
		m_PipelineLayout->UploadTexture(name,texture, set, binding);
	}

    void VulkanShader::SetTexture(std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding)
    {
        m_RenderAPI->SetTexture(texture,set,binding,m_Name);
    }
	
   
	std::vector<char> VulkanShader::readFile(const std::string& filepath)
	{
		std::string enginePath = filepath;
		std::ifstream file{ enginePath, std::ios::ate | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file: " + enginePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}
	void VulkanShader::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(VulkanDevice::GetVulkanDevice()->device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module");
		}
	}
	
}