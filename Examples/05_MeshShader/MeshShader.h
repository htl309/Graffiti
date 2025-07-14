#pragma once

#include<Graffiti.h>
#include"MeshShaderModel.h"
namespace Graffiti {
    class MeshShaderLayer : public Layer {
    public:

        MeshShaderLayer();
        ~MeshShaderLayer();

        void OnImGuiRender() override;
        void OnUpdate(TimeStep& ts) override;
        void OnEvent(Event& event) override;

        void MakePipeLine(std::shared_ptr<PipelineConfigInfo> MeshShader_configInfo, PipelineState state);
        void MeshShaderSubmit(uint32_t taskcount, const std::shared_ptr<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0));
    private:
        std::string m_Name;
        std::shared_ptr <ShaderLibrary> m_ShaderLibrary;

        SceneData m_Scene;

       
        std::shared_ptr <VertexArray> m_BunnyVertexArray;
        std::shared_ptr <VertexArray> m_DeerVertexArray;

        MeshShaderModel meshDeer;
        MeshShaderModel meshBunny;

        CameraControl m_CameraControl;


        bool IsWireFrame = 0;
        int model = 1;
    };


}