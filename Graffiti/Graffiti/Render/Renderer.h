#pragma once
#include"RenderCommand.h"
#include"Graffiti/base/Camera.h"
#include"Graffiti/base/ModelLibrary.h"
#include"Shader.h"
namespace Graffiti {

    class Render {
    public:
        
        static void Init(uint32_t contextindex = 0);

        static void BeginScene(SceneData& scenedata);

        static void OnWindowResize(uint32_t width, uint32_t height);
        static void EndScene();
        static void SetMode(bool wireframe);

        static uint32_t ContextIndex();


        static void Submit(const std::shared_ptr<VertexArray>& vertexarray,const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
        static void Submit(const std::shared_ptr<ModelLibrary>& modellib,const std::shared_ptr<Shader>& shader);
        inline static RenderAPI::API GetRenderAPI() { return RenderAPI::GetRenderAPI(); }
    private:
        static SceneData m_SceneData;
    };
}