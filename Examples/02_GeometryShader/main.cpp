
#include<Graffiti.h>

namespace Graffiti {
  
    class GeometryShaderLayer : public Layer {
    public:

        GeometryShaderLayer() :Layer("02_GeometryShader"), m_Name("02_GeometryShader") {
            
            m_ShaderLibrary = std::make_shared<ShaderLibrary>();

            //Bunny模型加载//////////////////////////////////////////////////////////////////////////////////
            std::shared_ptr<Model>   deer ;
            Model::LoadGLTFModel("Models/deer.gltf",deer);

            m_DeerVertexArray = VertexArray::Create();
            m_DeerVertexArray->AddModel(deer);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
            auto shader = Shader::Create(m_Name, {
                    {ShaderType::VertexShader, "02_GeometryShader/GeometryShader.vert"},
                    {ShaderType::GeometryShader, "02_GeometryShader/GeometryShader.geom"},
                    {ShaderType::FragmentShader, "02_GeometryShader/GeometryShader.frag"}
                }
            );  

            m_ShaderLibrary->Add(m_Name, shader);

            m_ShaderLibrary->Get(m_Name)->UploadUniformBuffer("NormalLength", sizeof(float), 1,1,0);
        
            std::shared_ptr<PipelineConfigInfo> configInfo = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame);
            m_ShaderLibrary->Get(m_Name)->AddPipeline(configInfo);
            m_ShaderLibrary->Get(m_Name)->Link();        
      
            m_ShaderLibrary->Add("Model", Shader::Create("Model", {
                {ShaderType::VertexShader, "02_GeometryShader/Model.vert"},
                {ShaderType::FragmentShader, "02_GeometryShader/Model.frag"}
                }
            ));
            std::shared_ptr<PipelineConfigInfo> configInfo2 = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame);
            m_ShaderLibrary->Get("Model")->AddPipeline(configInfo2);
           
            m_ShaderLibrary->Get("Model")->Link();
        
        }

        void OnImGuiRender() override {
            ImGui::Begin(m_Name.data());
            ImGui::SetWindowPos(ImVec2(40, 200), ImGuiCond_Always);
            ImGui::SetWindowSize(ImVec2(420, 180), ImGuiCond_Always);
          

            ImGui::Checkbox("WireFrameMode", &IsWireFrame);
         
            ImGui::SliderFloat("Length", &noramllenght, 0.0f, 0.6f);
            ImGui::Checkbox("Normal", &display_Normal);
            ImGui::SameLine();
            ImGui::Checkbox("Model", &display_Model);
            ImGui::End();
        }
        void OnUpdate(TimeStep& ts) override {
           
            if (!ImGui::GetIO().WantCaptureMouse) {
                // 只有在 ImGui 没有占用鼠标时，才允许相机控制逻辑生效
                m_CameraControl.KeyMovement(ts);
                m_CameraControl.MouseMovement(ts);
            }
          
            m_Scene.ViewProjectionMatrix = m_CameraControl.GetViewProjectionMatrix();
            m_Scene.CameraPosition = m_CameraControl.GetCameraPosition();

            Render::BeginScene(m_Scene);
            Render::SetMode(IsWireFrame);
         
           m_ShaderLibrary->Get(m_Name)->SetUniformBuffer("NormalLength", &noramllenght);
          
          if(display_Normal) Render::Submit(m_DeerVertexArray, m_ShaderLibrary->Get(m_Name));
          if(display_Model) Render::Submit(m_DeerVertexArray, m_ShaderLibrary->Get("Model"));
     
            
            Render::EndScene();
        }

        void OnEvent(Event& event) override {
            m_CameraControl.OnEvent(event);
        }
    private:
        std::string m_Name;
        std::shared_ptr < ShaderLibrary> m_ShaderLibrary;

        CameraControl m_CameraControl;
        SceneData m_Scene;
        PointLight light;

        bool IsWireFrame = 0;
        bool display_Normal = 1;
        bool display_Model = 1;

        float noramllenght=0.2;
   
        std::shared_ptr <VertexArray> m_DeerVertexArray;

    };


    class myApplication :public Application {
    public:
        myApplication() {

            PushLayer(new GeometryShaderLayer());
        }
        ~myApplication() {
            GF_INFO("myApplication Delete");
        }
    };

    Application* CreateApplication() {
        return new myApplication();
    }


}
#define GF_PLATFORM_WINDOWS 

#define GF_RENDERAPI_VULKAN
#define GF_RENDERAPI_OPENGL

#include"Graffiti/EntryPoint.h"

