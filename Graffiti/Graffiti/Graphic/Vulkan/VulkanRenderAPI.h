#pragma once
#include"Graffiti/Render/RenderAPI.h"
#include"Graffiti/base/core.h"
#include"VulkanContext.h"
#include"VulkanPipeline.h"
namespace Graffiti {

    class VulkanRenderAPI :public RenderAPI {
        friend class VulkanShader;
        friend class VulkanVertexBuffer;
        friend class VulkanIndexBuffer;
    public:
   
        virtual void Init(uint32_t contextindex = 0) override;

        virtual void SetDepthtest(bool set) override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndex(const std::shared_ptr<VertexArray>& vertexarray)  override;

        virtual void WireframeMode() override;
        virtual void PolygonMode() override;
       
    private:
        void BindPipeline(const std::string& PipelineName);
        void SetTexture(std::shared_ptr<Texture> texture, uint32_t set, uint32_t binding, const std::string& PipelineName);

    private:
        std::shared_ptr<VulkanContext> m_VulkanContext;

        //存放管线的地方
        std::unordered_map<std::string, std::vector<VulkanPipeline>> m_PipelineMap;

        PipelineState m_State=PipelineState::Common;
    };
}