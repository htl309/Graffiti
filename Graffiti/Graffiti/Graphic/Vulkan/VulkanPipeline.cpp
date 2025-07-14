#include"gfpch.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "Graffiti/Render/Renderer.h"
namespace Graffiti {



    PipelineConfigInfo::PipelineConfigInfo(PipelineState state)
       : m_State(state)
    {
        SetInfo(m_State);
    }

    void PipelineConfigInfo::SetInfo(PipelineState state) {
        viewportInfo = new VkPipelineViewportStateCreateInfo();
        inputAssemblyInfo = new VkPipelineInputAssemblyStateCreateInfo();
        rasterizationInfo = new VkPipelineRasterizationStateCreateInfo();
        multisampleInfo = new VkPipelineMultisampleStateCreateInfo();
        colorBlendAttachment = new VkPipelineColorBlendAttachmentState();
        colorBlendInfo = new VkPipelineColorBlendStateCreateInfo();
        depthStencilInfo = new VkPipelineDepthStencilStateCreateInfo();
        dynamicStateInfo = new VkPipelineDynamicStateCreateInfo();

        inputAssemblyInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo->topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyInfo->primitiveRestartEnable = VK_FALSE;

        viewportInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo->viewportCount = 1;
        viewportInfo->pViewports = nullptr;
        viewportInfo->scissorCount = 1;
        viewportInfo->pScissors = nullptr;

        rasterizationInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationInfo->depthClampEnable = VK_FALSE;
        rasterizationInfo->rasterizerDiscardEnable = VK_FALSE;

        rasterizationInfo->polygonMode = HasState(state, PipelineState::WireFrame) ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
        rasterizationInfo->lineWidth = 1.0f;
        rasterizationInfo->cullMode = VK_CULL_MODE_NONE;
        rasterizationInfo->frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationInfo->depthBiasEnable = VK_FALSE;
        rasterizationInfo->depthBiasConstantFactor = 0.0f;  // Optional
        rasterizationInfo->depthBiasClamp = 0.0f;           // Optional
        rasterizationInfo->depthBiasSlopeFactor = 0.0f;     // Optional

        multisampleInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo->sampleShadingEnable = VK_FALSE;
        multisampleInfo->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo->minSampleShading = 1.0f;           // Optional
        multisampleInfo->pSampleMask = nullptr;             // Optional
        multisampleInfo->alphaToCoverageEnable = VK_FALSE;  // Optional
        multisampleInfo->alphaToOneEnable = VK_FALSE;       // Optional

        colorBlendAttachment->colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;


        colorBlendAttachment->blendEnable = VK_TRUE;
        colorBlendAttachment->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment->colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment->alphaBlendOp = VK_BLEND_OP_ADD;

        colorBlendInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo->logicOpEnable = VK_FALSE;
        colorBlendInfo->logicOp = VK_LOGIC_OP_COPY;  // Optional
        colorBlendInfo->attachmentCount = 1;
        colorBlendInfo->pAttachments = colorBlendAttachment;
        colorBlendInfo->blendConstants[0] = 0.0f;  // Optional
        colorBlendInfo->blendConstants[1] = 0.0f;  // Optional
        colorBlendInfo->blendConstants[2] = 0.0f;  // Optional
        colorBlendInfo->blendConstants[3] = 0.0f;  // Optional

        depthStencilInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilInfo->depthTestEnable = VK_TRUE;
        depthStencilInfo->depthWriteEnable = HasState(state, PipelineState::DepthTestDisable) ? VK_FALSE : VK_TRUE;
        depthStencilInfo->depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilInfo->depthBoundsTestEnable = VK_FALSE;
        depthStencilInfo->minDepthBounds = 0.0f;  // Optional
        depthStencilInfo->maxDepthBounds = 1.0f;  // Optional
        depthStencilInfo->stencilTestEnable = VK_FALSE;
        depthStencilInfo->front = {};  // Optional
        depthStencilInfo->back = {};   // Optional

        dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        dynamicStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicStateInfo->pDynamicStates = dynamicStateEnables.data();
        dynamicStateInfo->dynamicStateCount =
            static_cast<uint32_t>(dynamicStateEnables.size());
        dynamicStateInfo->flags = 0;


        bindingDescriptions = VulkanPipeline::getBindingDescriptions();
        attributeDescriptions = VulkanPipeline::getAttributeDescriptions();
    }

    PipelineConfigInfo::~PipelineConfigInfo()
    {
        delete viewportInfo;
        delete inputAssemblyInfo;
        delete rasterizationInfo;
        delete multisampleInfo;
        delete colorBlendAttachment;
        delete colorBlendInfo;
        delete depthStencilInfo;
        delete dynamicStateInfo;
    }


    void VulkanPipeline::SetPushConstants(uint32_t size, const void* data)
    {
        auto context = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[Render::ContextIndex()]);
        vkCmdPushConstants(
            context->getCurrentCommandBuffer(),
            GetVkPipelineLayout(),
            VK_SHADER_STAGE_ALL_GRAPHICS | VK_SHADER_STAGE_TASK_BIT_NV | VK_SHADER_STAGE_MESH_BIT_NV,
            0,
            size,
            data);
    }

    void VulkanPipelineLayout::UploadUniform(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding)
    {
        std::shared_ptr<UniformBuffer> buffer = UniformBuffer::Create(size, count, set, binding); 
        m_UniformBuffer[name] = buffer;
        if (set >= m_SetCount) {
            m_SetCount = set + 1;
            m_DescriptorSetLayoutsInfos.push_back(VulkanDescriptorSetLayout::Builder()); 
        }
        m_DescriptorSetLayoutsInfos[set].addBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS| VK_SHADER_STAGE_TASK_BIT_NV | VK_SHADER_STAGE_MESH_BIT_NV);
    }

    void VulkanPipelineLayout::UploadStorage(const std::string& name, uint32_t size, uint32_t count, uint32_t set, uint32_t binding)
    {
        std::shared_ptr<StorageBuffer> buffer = StorageBuffer::Create(size, count, set, binding);
        m_StorageBuffer[name] = buffer;
        if (set >= m_SetCount) {
            m_SetCount = set + 1;
            m_DescriptorSetLayoutsInfos.push_back(VulkanDescriptorSetLayout::Builder());
        }
        m_DescriptorSetLayoutsInfos[set].addBinding(binding, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS | VK_SHADER_STAGE_TASK_BIT_NV | VK_SHADER_STAGE_MESH_BIT_NV);

    }

    void VulkanPipelineLayout::UploadTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding) {
       
        std::shared_ptr<VulkanTexture> temptexture = std::dynamic_pointer_cast<VulkanTexture>(texture);
        temptexture->SetSet(set);
        temptexture->SetBinding(binding);
        m_Texture[name] = texture;
     
        if (set >= m_SetCount) {
            m_SetCount = set + 1;
            m_DescriptorSetLayoutsInfos.push_back(VulkanDescriptorSetLayout::Builder());
        }
        m_DescriptorSetLayoutsInfos[set].addImageBinding(binding);
    }
  
    
    std::vector<VkVertexInputBindingDescription> VulkanPipeline::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    std::vector<VkVertexInputAttributeDescription> VulkanPipeline::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,    offsetof(Vertex, position) });
        attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT,    offsetof(Vertex, normal) });
        attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32_SFLOAT,       offsetof(Vertex, texCoord) });
     //   attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32B32_SFLOAT,       offsetof(Vertex, tangent) });

        return attributeDescriptions;
    }

    
    void VulkanPipelineLayout::buildDescriptorSets()
    {
        m_DescriptorSets.resize(m_SetCount);
        m_DescriptorSetPools.resize(m_SetCount);
        m_DescriptorSetLayouts.resize(m_SetCount);

       m_DescriptorWriter.resize(m_SetCount);


        for (uint32_t i = 0; i < m_SetCount; ++i) {

            m_DescriptorSetLayouts[i] = m_DescriptorSetLayoutsInfos[i].build();
            uint32_t tempsetNumber = m_DescriptorSetLayoutsInfos[i].bindings.size();
            VulkanDescriptorPool::Builder DescriptorSetPoolsBuilder = VulkanDescriptorPool::Builder();
                DescriptorSetPoolsBuilder.setMaxSets(tempsetNumber*100);
            for (int j = 0; j < tempsetNumber; j++)
            {
                DescriptorSetPoolsBuilder.addPoolSize(
                    m_DescriptorSetLayoutsInfos[i].bindings[j].descriptorType, tempsetNumber*100);  
            }
            m_DescriptorSetPools[i] = DescriptorSetPoolsBuilder.build();
            m_DescriptorWriter[i] = std::make_shared<VulkanDescriptorWriter>(*m_DescriptorSetLayouts[i], *m_DescriptorSetPools[i]);
        }

        /* for (auto pair : m_Data) {
                 std::shared_ptr<VulkanUniformBuffer>& buffer = std::dynamic_pointer_cast<VulkanUniformBuffer>(pair.second);
                 GF_CORE_TRACE("{0},{1}",pair.first, buffer->m_Set);
                 DescriptorWriter[buffer->m_Set]->writeBuffer(
                     buffer->m_Binding,
                     &buffer->GetDescriptorInfo());
         }*/
         
        for (uint32_t i = 0; i < m_SetCount; ++i) { 
            for (auto pair : m_UniformBuffer) {

                if (pair.second->m_Set == i)
                {
                    std::shared_ptr<VulkanUniformBuffer> buffer = std::dynamic_pointer_cast<VulkanUniformBuffer>(pair.second);
                    m_DescriptorWriter[i]->writeBuffer(
                        buffer->m_Binding,
                        &buffer->GetDescriptorInfo());  
                }
            }
            for (auto pair : m_StorageBuffer) {

                if (pair.second->m_Set == i)
                {
                    std::shared_ptr<VulkanStorageBuffer> buffer = std::dynamic_pointer_cast<VulkanStorageBuffer>(pair.second);
                    m_DescriptorWriter[i]->writeBuffer(
                        buffer->m_Binding,
                        &buffer->GetDescriptorInfo());
         
                }
            }
            for (auto pair : m_Texture) {
                std::shared_ptr<VulkanTexture> texture = std::dynamic_pointer_cast<VulkanTexture>(pair.second);
                if (texture->GetSet() == i)
                {
                    m_DescriptorWriter[i]->writeImage(
                        texture->GetBinding(),
                        &texture->GetDescriptorImageInfo()); 
                }
            }

            m_DescriptorWriter[i]->build(m_DescriptorSets[i]);
        }
    }

    void VulkanPipelineLayout::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS | VK_SHADER_STAGE_TASK_BIT_NV | VK_SHADER_STAGE_MESH_BIT_NV;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(glm::mat4);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts(m_SetCount);
        for (int i = 0; i < m_SetCount; i++) {
            descriptorSetLayouts[i] = m_DescriptorSetLayouts[i]->getDescriptorSetLayout();
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(VulkanDevice::GetVulkanDevice()->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

}