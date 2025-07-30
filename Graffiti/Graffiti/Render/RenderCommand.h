#pragma once
#include"RenderAPI.h"
#include "Graffiti/ImGui/imgui.h"
namespace Graffiti {

    class RenderCommand {
    public:
        inline static void Init(uint32_t contextindex = 0) {
            Create();
            s_RenderAPI->Init(contextindex);
        }
        inline static uint32_t ContextIndex() {
            return  s_RenderAPI->ContextIndex;
        }
        inline static void SetDepthtest(bool set) {
            s_RenderAPI->SetDepthtest(set);
        }
        inline static void SetMeshShader(bool set) {
            s_RenderAPI->SetMeshShader(set);
        }
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RenderAPI->SetViewport(x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color) {
            s_RenderAPI->SetClearColor(color);
        }
        inline static void Clear() {
            s_RenderAPI->Clear();
        }
        inline static void DrawIndex(const std::shared_ptr<VertexArray>& vertexarray) {
            s_RenderAPI->DrawIndex(vertexarray);    
        }
        inline static void MeshShaderDraw(uint32_t taskcount) {
            s_RenderAPI->MeshShaderDraw(taskcount);
        }

        inline static void WireframeMode() {
            s_RenderAPI->WireframeMode();
        }
        inline static void PolygonMode() {
            s_RenderAPI->PolygonMode();
        }
        static ImTextureID GetTextureID(std::shared_ptr<Texture> Texture);

        static void Create();
        static RenderAPI* GetRenderAPI();
    private:
       static RenderAPI* s_RenderAPI;
       

    };
} 