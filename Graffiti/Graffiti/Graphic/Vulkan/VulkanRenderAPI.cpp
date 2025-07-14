#include "VulkanRenderAPI.h"
#include "Graffiti/base/Window.h"


void Graffiti::VulkanRenderAPI::Init(uint32_t contextindex)
{
	RenderAPI::ContextIndex = contextindex;
	m_VulkanContext = std::dynamic_pointer_cast<VulkanContext>(GraphicsContext::G_Context[ContextIndex]);
    vkCmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)vkGetDeviceProcAddr(VulkanDevice::GetVulkanDevice()->device(), "vkCmdDrawMeshTasksNV");
}

void SetDepthtest(bool set) {
    if (set) {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void Graffiti::VulkanRenderAPI::SetDepthtest(bool set)
{
    if(set)
        m_State = m_State & ~PipelineState::DepthTestDisable;
    else
        m_State = m_State | PipelineState::DepthTestDisable;
}

void Graffiti::VulkanRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
}

void Graffiti::VulkanRenderAPI::SetClearColor(const glm::vec4& color)
{
}

void Graffiti::VulkanRenderAPI::Clear()
{
}

void Graffiti::VulkanRenderAPI::DrawIndex(const std::shared_ptr<VertexArray>& vertexarray)
{
	vkCmdDrawIndexed(m_VulkanContext->getCurrentCommandBuffer(), vertexarray->GetIndexBuffer()->GetCount(), 1, 0, 0, 0);
}
void Graffiti::VulkanRenderAPI::MeshShaderDraw(uint32_t taskcount)
{
    vkCmdDrawMeshTasksNV(m_VulkanContext->getCurrentCommandBuffer(), taskcount, 0);
}
void Graffiti::VulkanRenderAPI::WireframeMode()
{
    m_State = m_State | PipelineState::WireFrame;
}

void Graffiti::VulkanRenderAPI::PolygonMode()
{
    m_State = m_State & ~PipelineState::WireFrame;
}

void Graffiti::VulkanRenderAPI::BindPipeline(const std::string& PipelineName)
{
 
    for (int i = 0; i < m_PipelineMap[PipelineName].size();i++) {
       
        if(m_PipelineMap[PipelineName][i].IsEqualState(m_State))
        {
            vkCmdBindPipeline(m_VulkanContext->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineMap[PipelineName][i].m_Pipeline);
            vkCmdBindDescriptorSets(
                m_VulkanContext->getCurrentCommandBuffer(),
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                m_PipelineMap[PipelineName][i].GetVkPipelineLayout(),
                0,
                m_PipelineMap[PipelineName][i].GetDescriptorSetCount(),
                m_PipelineMap[PipelineName][i].GetDescriptorSetData(),
                0,
                nullptr); 
            return;
        }
    }

}

void Graffiti::VulkanRenderAPI::SetTexture( std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding, const std::string& PipelineName, const std::string& ModelName)
{

    std::shared_ptr<VulkanTexture> temptexture = std::dynamic_pointer_cast<VulkanTexture>(texture);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = temptexture->GetImageView();
    imageInfo.sampler = temptexture->GetSampler();

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  
    write.dstBinding = binding; // 和 shader 中的 binding = 0 对应
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &imageInfo;



    if (!ModelName.empty()) {

        if (m_new_DescriptorSet.find(ModelName) == m_new_DescriptorSet.end()) {
           
            VkDescriptorSet newVkDescriptorSet;
            m_PipelineMap[PipelineName][0].GetDescriptorPool(set)->allocateDescriptor(
                m_PipelineMap[PipelineName][0].GetDescriptorSetLayout(set)->descriptorSetLayout, 
                newVkDescriptorSet);    
     
            m_new_DescriptorSet.insert({ ModelName ,newVkDescriptorSet }); 
        }  
        write.dstSet = m_new_DescriptorSet[ModelName];
        vkUpdateDescriptorSets(VulkanDevice::GetVulkanDevice()->device(), 1, &write, 0, nullptr);

        for (int i = 0; i < m_PipelineMap[PipelineName].size(); i++) {
            if (m_PipelineMap[PipelineName][i].IsEqualState(m_State))
            {
                vkCmdBindPipeline(m_VulkanContext->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineMap[PipelineName][i].m_Pipeline);
                vkCmdBindDescriptorSets(
                    m_VulkanContext->getCurrentCommandBuffer(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    m_PipelineMap[PipelineName][i].GetVkPipelineLayout(),
                    set,
                    1,
                    &m_new_DescriptorSet[ModelName],
                    0,
                    nullptr);
                return;
            }

        }
    }
    else {
        for (int i = 0; i < m_PipelineMap[PipelineName].size(); i++) {
            if (m_PipelineMap[PipelineName][i].IsEqualState(m_State))
            {
                write.dstSet = m_PipelineMap[PipelineName][i].GetDescriptorSet(set);
                vkUpdateDescriptorSets(VulkanDevice::GetVulkanDevice()->device(), 1, &write, 0, nullptr);
                return;
            }
        }
    }

}


