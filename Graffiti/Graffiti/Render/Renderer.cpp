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
   void Render::Submit(const std::shared_ptr<ModelLibrary>& modellib, const std::shared_ptr<Shader>& shader) {

       for (int i = 0; i < modellib->m_VertexArrays.size(); i++) {
           shader->Bind();
           shader->SetSceneData(m_SceneData);
           shader->SetTransform(modellib->m_Models[i]->m_Transform);
           modellib->m_VertexArrays[i]->Bind();
     
           Material& material = modellib->m_Models[i]->m_Material; 

           if(material.baseColorTexID != -1)
           shader->SetTexture( modellib->m_Textures[material.baseColorTexID], 1, 0, modellib->m_Models[i]->m_Name);
           if (material.metallicRoughTexID != -1)
           shader->SetTexture(modellib->m_Textures[material.metallicRoughTexID], 1, 1, modellib->m_Models[i]->m_Name);
           if (material.normalTexID !=-1)
           shader->SetTexture(modellib->m_Textures[material.normalTexID], 1, 2, modellib->m_Models[i]->m_Name);

           RenderCommand::DrawIndex(modellib->m_VertexArrays[i]);   
       }

    }
  
}