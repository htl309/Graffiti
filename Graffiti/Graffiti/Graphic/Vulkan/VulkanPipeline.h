#pragma once

#include "VulkanDevice.h"
#include "Graffiti/base/PipeLineState.h"
#include "Graffiti/Scene/Vertex.h"
#include "Graffiti/Scene/SceneData.h"

#include "VulkanBuffer.h"
#include "VulkanDescriptor.h"
#include "VulkanSwapChain.h"
#include "VulkanTexture.h"

namespace Graffiti {
    
    class VulkanPipelineLayout {
        friend class PipelineConfigInfo;
        friend class VulkanShader;
    public:

        VulkanPipelineLayout() {};
        void UploadUniform(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding);
        void UploadStorage(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding);
        void UploadTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding);
        //这里的vector的单元是和set一一对应，也就是说vector[0]对应的是set=0；
        std::vector <VkDescriptorSet> m_DescriptorSets;
        std::vector <std::shared_ptr <VulkanDescriptorPool>> m_DescriptorSetPools;
        std::vector <VulkanDescriptorSetLayout::Builder> m_DescriptorSetLayoutsInfos;
        std::vector <std::shared_ptr <VulkanDescriptorSetLayout>> m_DescriptorSetLayouts;
        std::vector<std::shared_ptr	<VulkanDescriptorWriter>> m_DescriptorWriter;

        VkPipelineLayout m_PipelineLayout;
        //虽然初始化等于0，但是实际上肯定有1，因为得传SceneData
        uint32_t m_SetCount = 0;

        //Buffer
        //前一个是名字，后一个是buffer
        std::unordered_map<std::string, std::shared_ptr<UniformBuffer> > m_UniformBuffer;
        std::unordered_map<std::string, std::shared_ptr<StorageBuffer> > m_StorageBuffer;
        std::unordered_map<std::string, std::shared_ptr<Texture> > m_Texture;
    private:
        void buildDescriptorSets();
        void createPipelineLayout();

    };
    class PipelineConfigInfo {
    public:
        PipelineConfigInfo() {};
        PipelineConfigInfo(PipelineState state) ;
        ~PipelineConfigInfo();

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo* viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo* inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo* rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo* multisampleInfo;
        VkPipelineColorBlendAttachmentState* colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo* colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo* depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo* dynamicStateInfo;
       
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
        std::shared_ptr<VulkanPipelineLayout> m_VulkanPipelineLayout;
        PipelineState m_State;
    private:
        void SetInfo(PipelineState state);
    };
	class VulkanPipeline {
        friend class VulkanRenderAPI;
    public:
        ~VulkanPipeline() {  }
        VulkanPipeline::VulkanPipeline(std::vector<VkPipelineShaderStageCreateInfo> ShaderStages, std::shared_ptr<PipelineConfigInfo>  configInfo)
        {
            m_PipelineConfigInfo = configInfo;

            if (configInfo == nullptr) GF_TRACE("nullptr");
            assert(configInfo->m_VulkanPipelineLayout->m_PipelineLayout != VK_NULL_HANDLE &&
                "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
            assert(configInfo->renderPass != VK_NULL_HANDLE &&
                "Cannot create graphics pipeline: no renderPass provided in configInfo");

            auto& bindingDescriptions = configInfo->bindingDescriptions;
            auto& attributeDescriptions = configInfo->attributeDescriptions;
            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexAttributeDescriptionCount =
                static_cast<uint32_t>(attributeDescriptions.size());
            vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
            vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
            vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();


            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = ShaderStages.size();
            pipelineInfo.pStages = ShaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = configInfo->inputAssemblyInfo;
            pipelineInfo.pViewportState = configInfo->viewportInfo;
            pipelineInfo.pRasterizationState = configInfo->rasterizationInfo;
            pipelineInfo.pMultisampleState = configInfo->multisampleInfo;
            pipelineInfo.pColorBlendState = configInfo->colorBlendInfo;
            pipelineInfo.pDepthStencilState = configInfo->depthStencilInfo;
            pipelineInfo.pDynamicState = configInfo->dynamicStateInfo;


            pipelineInfo.layout = configInfo->m_VulkanPipelineLayout->m_PipelineLayout;
            pipelineInfo.renderPass = configInfo->renderPass;
            pipelineInfo.subpass = configInfo->subpass;  
            if (vkCreateGraphicsPipelines(
                VulkanDevice::GetVulkanDevice()->device(),
                VK_NULL_HANDLE,
                1,
                &pipelineInfo,
                nullptr,
                &m_Pipeline) != VK_SUCCESS) {
                throw std::runtime_error("failed to create graphics pipeline");
            } 
        }
      
        void SetPushConstants(uint32_t size, const void* data);
      
        inline void SetPipeLineState(PipelineState& state) { m_PipelineConfigInfo->m_State = state; }
        inline bool IsInState(PipelineState state) { return HasState(m_PipelineConfigInfo->m_State , state); }
        inline bool IsEqualState(PipelineState state) { return m_PipelineConfigInfo->m_State == state; }
        inline VkPipelineLayout  GetVkPipelineLayout() { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_PipelineLayout; }
        inline  uint32_t GetDescriptorSetCount() { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorSets.size(); }
        inline  const VkDescriptorSet* GetDescriptorSetData() { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorSets.data(); }
        inline  const VkDescriptorSet GetDescriptorSet(uint32_t index) { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorSets[index]; }
        inline std::shared_ptr	<VulkanDescriptorWriter> GetDescriptorWriter(uint32_t index) { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorWriter[index]; }
        inline std::shared_ptr	<VulkanDescriptorPool> GetDescriptorPool(uint32_t index) { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorSetPools[index]; }
        inline std::shared_ptr	<VulkanDescriptorSetLayout> GetDescriptorSetLayout(uint32_t index) { return m_PipelineConfigInfo->m_VulkanPipelineLayout->m_DescriptorSetLayouts[index]; }

  
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        
        
    private:

        VkPipeline m_Pipeline = VK_NULL_HANDLE;

        std::shared_ptr<PipelineConfigInfo>  m_PipelineConfigInfo;
  
	};

}