#include"gfpch.h"
#include"Renderer.h"

namespace Graffiti {

    SceneData  Render::m_SceneData;
    void Render::Init(uint32_t contextindex)
    {
        RenderCommand::Init(contextindex);
    }

  
    void Render::BeginScene(SceneData& scenedata)
    {
        m_SceneData = scenedata;
        RenderCommand::Clear();
    }
    void Render::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Render::EndScene()
    {
    }
    void Render::SetMode(bool wireframe) {
        if (wireframe) {
            RenderCommand::WireframeMode();
        }
        else {
            RenderCommand::PolygonMode();
        }

    }
    uint32_t Render::ContextIndex()
    {
        return RenderCommand::ContextIndex();
    }
  

    void  Render::Submit(const std::shared_ptr<VertexArray>& vertexarray, const std::shared_ptr<Shader>& shader , const glm::mat4& transform )
    {
     
        shader->Bind(); 
        shader->SetSceneData(m_SceneData);
        shader->SetTransform(transform); 
        vertexarray->Bind(); 
        RenderCommand::DrawIndex(vertexarray); 
    }
  
}