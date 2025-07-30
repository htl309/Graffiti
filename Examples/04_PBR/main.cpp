
#include<Graffiti.h>

#include "Graffiti/Scene/Model.h"
namespace Graffiti {

    class Bunny : public Layer {
    public:

        Bunny() :Layer("04_PBR"), m_Name("04_PBR") {

            m_ShaderLibrary = std::make_shared<ShaderLibrary>();

            ModelLibrary::LoadGLTFModel("Models/chess/chess_set_4k.gltf", m_chess);
            ModelLibrary::LoadGLTFModel("Models/chair/mid_century_lounge_chair_4k.gltf", m_chair);
            ModelLibrary::LoadGLTFModel("Models/lion/lion_head_4k.gltf", m_sign);
          

            
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
            auto shader = Shader::Create(m_Name, {
                    {ShaderType::VertexShader, "04_PBR/PBR.vert"},
                    {ShaderType::FragmentShader, "04_PBR/PBR.frag"}
                }
            );

            std::shared_ptr<PipelineConfigInfo> configInfo = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame);
            shader->AddPipeline(configInfo);

            //Vulkan创建管线的时候就要预先绑定好纹理了，所以这里先绑定一张无关紧要的纹理
            std::shared_ptr<Texture>  t = Texture::Create("resource\\logo\\Graffiti_LOGO2.png");
            shader->UploadTexture("TexSampler", t, 1, 0);
            shader->UploadTexture("ARMTex", t, 1, 1);
            shader->UploadTexture("NormalTex", t, 1, 2);

            m_ShaderLibrary->Add(m_Name, shader);
            m_ShaderLibrary->Get(m_Name)->Link();
    
            m_CameraControl.SetTarget(m_chess->boundingbox);

            m_Scene.Light.position = glm::vec4(1, 1, 1, 1.0f);
        }

        void OnImGuiRender() override {
            ImGui::Begin(m_Name.data());
            ImGui::SetWindowPos(ImVec2(40, 200), ImGuiCond_Always);
            ImGui::SetWindowSize(ImVec2(420, 180), ImGuiCond_Always);

            ImGui::Checkbox("WireFrameMode", &IsWireFrame);
            ImGui::SameLine();
            if (ImGui::Button("ClearScene", ImVec2(160, 38))) {
                model = -1;
            }

            ImGui::Text("Models: ");
            ImGui::SameLine();
            if (ImGui::Button("Chair", ImVec2(80, 35))) {
                model = 0;
                m_CameraControl.SetTarget(m_chair->boundingbox);         
            }
            ImGui::SameLine();
            if (ImGui::Button("Chess", ImVec2(80, 35))) {
                model = 1;
                m_CameraControl.SetTarget(m_chess->boundingbox);
            }
            ImGui::SameLine();
            if (ImGui::Button("Sign", ImVec2(100, 35))) {
                model = 2;
                m_CameraControl.SetTarget(m_sign->boundingbox);
            }

            ImGui::End();
        }
        void OnUpdate(TimeStep& ts) override {

            if (!ImGui::GetIO().WantCaptureMouse) {
                // 只有在 ImGui 没有占用鼠标时，才允许相机控制逻辑生效
                m_CameraControl.KeyMovement(ts);
                m_CameraControl.MouseMovement();
            }

            m_Scene.ViewProjectionMatrix = m_CameraControl.GetViewProjectionMatrix();
            m_Scene.CameraPosition = m_CameraControl.GetCameraPosition();
    
            Render::BeginScene(m_Scene);
            Render::SetMode(IsWireFrame);
         
            if (model == 0)    Render::Submit(m_chair, m_ShaderLibrary->Get(m_Name));
            else if (model == 1) Render::Submit(m_chess, m_ShaderLibrary->Get(m_Name));
            else if (model == 2) Render::Submit(m_sign, m_ShaderLibrary->Get(m_Name));

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
        int model = 1;

        
        std::shared_ptr <ModelLibrary> m_chess;
        std::shared_ptr <ModelLibrary> m_chair;
        std::shared_ptr <ModelLibrary> m_sign;

    };


    class BunnyApplication :public Application {
    public:
        BunnyApplication() {

            PushLayer(new Bunny());
        }
        ~BunnyApplication() {
        }
    };

    Application* CreateApplication() {
        return new BunnyApplication();
    }


}
#define GF_PLATFORM_WINDOWS 

#define GF_RENDERAPI_VULKAN


 #define GF_RENDERAPI_OPENGL

#include"Graffiti/EntryPoint.h"

