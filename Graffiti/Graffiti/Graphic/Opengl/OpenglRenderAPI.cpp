#include"OpenGLRenderAPI.h"
#include<glad/glad.h>
namespace Graffiti {

   
    void OpenGLRenderAPI::Init(uint32_t contextindex)
    {
        RenderAPI::ContextIndex = contextindex;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        SetClearColor({ 0.12, 0.12, 0.12, 1.0 });
    }
   void OpenGLRenderAPI::SetDepthtest(bool set) {
       if (set) {
           glEnable(GL_DEPTH_TEST);
       }
       else
       {
           glDisable(GL_DEPTH_TEST);
       }
    }
    void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
       glViewport(x, y, width, height);
    }
    void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
    {
        glDisable(GL_CULL_FACE);
        SetDepthtest(true);
        glDepthFunc(GL_LESS);
        glClearColor(color.r, color.g, color.b, color.w); 
    }
    void OpenGLRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
    void OpenGLRenderAPI::DrawIndex(const std::shared_ptr<VertexArray>& vertexarray)
    {
        glDrawElements(GL_TRIANGLES, vertexarray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderAPI::WireframeMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void OpenGLRenderAPI::PolygonMode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


}