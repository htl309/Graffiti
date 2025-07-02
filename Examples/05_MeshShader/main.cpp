
#include<Graffiti.h>

#include "Graffiti/Scene/Model.h"
namespace Graffiti {
    void createCube(std::shared_ptr<Model>  cube) {
        std::vector<Vertex>& vertices = cube->m_Vertices;
        std::vector<uint32_t>& indices = cube->m_Indices;
        vertices.resize(8);
        vertices[0].position = { -0.5f, -0.5f, -0.5f }; // 左下后
        vertices[1].position = { 0.5f, -0.5f, -0.5f }; // 右下后
        vertices[2].position = { 0.5f,  0.5f, -0.5f }; // 右上后
        vertices[3].position = { -0.5f,  0.5f, -0.5f }; // 左上后
        vertices[4].position = { -0.5f, -0.5f,  0.5f }; // 左下前
        vertices[5].position = { 0.5f, -0.5f,  0.5f }; // 右下前
        vertices[6].position = { 0.5f,  0.5f,  0.5f }; // 右上前
        vertices[7].position = { -0.5f,  0.5f,  0.5f }; // 左上前
        indices = {// 前面
                0, 1, 2,	2, 3, 0,
                // 后面
                5, 4, 7,7, 6, 5,
                // 左面
                4, 0, 3,3, 7, 4,
                // 右面
                1, 5, 6,6, 2, 1,
                // 上面
                4, 5, 1,1, 0, 4,
                // 下面
                3, 2, 6,6, 7, 3
        };
        cube->boundingbox.maxPos = glm::vec3(0.5, 0.5, 0.5);
        cube->boundingbox.minPos = glm::vec3(-0.5, -0.5, -0.5);

    }
    class Bunny : public Layer {
    public:

        Bunny() :Layer("01_Bunny"), m_Name("01_Bunny") {

            m_ShaderLibrary = std::make_shared<ShaderLibrary>();
            m_camera = std::make_shared<Camera>();

            //Bunny模型加载//////////////////////////////////////////////////////////////////////////////////
            std::shared_ptr<Model> bunny;
            Model::LoadGLTFModel("Models/bunny.gltf", bunny);

            m_BunnyVertexArray = VertexArray::Create();

            m_BunnyVertexArray->AddModel(bunny);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////
            std::shared_ptr<Model> deer;
            Model::LoadGLTFModel("Models/deer.gltf", deer);

            m_DeerVertexArray = VertexArray::Create();
            m_DeerVertexArray->AddModel(deer);

            //立方体模型加载//////////////////////////////////////////////////////////////////////////////////
            std::shared_ptr<Model>  cube = std::make_shared<Model>();
            createCube(cube);
            m_CubeVertexArray = VertexArray::Create();
            m_CubeVertexArray->AddModel(cube);
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            auto shader = Shader::Create(m_Name, {
                    {ShaderType::VertexShader, "01_Bunny/Bunny.vert"},
                    {ShaderType::FragmentShader, "01_Bunny/Bunny.frag"}
                }
            );
            m_ShaderLibrary->Add(m_Name, shader);
            m_ShaderLibrary->Get(m_Name)->Link();


            m_camera->SetTarget(m_BunnyVertexArray->m_Model->boundingbox);
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
            if (ImGui::Button("Cube", ImVec2(80, 35))) {
                model = 0;
                m_camera->SetTarget(m_CubeVertexArray->m_Model->boundingbox);
            }
            ImGui::SameLine();
            if (ImGui::Button("Bunny", ImVec2(80, 35))) {
                model = 1;
                m_camera->SetTarget(m_BunnyVertexArray->m_Model->boundingbox);
            }
            ImGui::SameLine();
            if (ImGui::Button("Deer", ImVec2(80, 35))) {
                model = 2;
                m_camera->SetTarget(m_DeerVertexArray->m_Model->boundingbox);
            }


            ImGui::End();
        }
        void OnUpdate(TimeStep& ts) override {
            if (Input::IsKeyPressed(GF_KEY_A))
                m_camera->ProcessKeyboard(CameraMoveDir::LEFT, ts);
            if (Input::IsKeyPressed(GF_KEY_S))
                m_camera->ProcessKeyboard(CameraMoveDir::DOWN, ts);
            if (Input::IsKeyPressed(GF_KEY_D))
                m_camera->ProcessKeyboard(CameraMoveDir::RIGHT, ts);
            if (Input::IsKeyPressed(GF_KEY_W))
                m_camera->ProcessKeyboard(CameraMoveDir::UP, ts);
            if (Input::IsKeyPressed(GF_KEY_UP))
                m_camera->ProcessKeyboard(CameraMoveDir::FOWARD, ts);
            if (Input::IsKeyPressed(GF_KEY_DOWN))
                m_camera->ProcessKeyboard(CameraMoveDir::BACKWARD, ts);
            if (!ImGui::GetIO().WantCaptureMouse) {
                if (Input::IsMouseButtonPressed(GF_MOUSE_BUTTON_LEFT)) {
                    if (m_firstMouse == true) {
                        lastX = Input::GetMouseX();
                        lastY = Input::GetMouseY();
                        m_firstMouse = false;
                    }

                    float dx = Input::GetMouseX() - lastX;
                    float dy = Input::GetMouseY() - lastY;

                    m_camera->ProcessMouseMovement(dx, dy, ts);
                    lastX = Input::GetMouseX();
                    lastY = Input::GetMouseY();
                }
                else {
                    m_firstMouse = true;
                }
            }


            m_Scene.ViewProjectionMatrix = m_camera->GetViewProjectionMatrix();
            m_Scene.CameraPosition = m_camera->GetCameraPosition();

            Render::BeginScene(m_Scene);
            Render::SetMode(IsWireFrame);

            if (model == 0)    Render::Submit(m_CubeVertexArray, m_ShaderLibrary->Get(m_Name));
            else if (model == 1) Render::Submit(m_BunnyVertexArray, m_ShaderLibrary->Get(m_Name));
            else if (model == 2) Render::Submit(m_DeerVertexArray, m_ShaderLibrary->Get(m_Name));




            Render::EndScene();
        }

        void OnEvent(Event& event) override {

            EventDispatcher dispatcher(event);
            dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(Bunny::OnWindowResize));
            if (event.GetEventType() == EventType::KeyPressed)
            {
                KeyPressedEvent& e = (KeyPressedEvent&)event;
                if (e.GetKeyCode() == GF_KEY_TAB)
                    GF_TRACE("Tab key is pressed (event)!");

            }

        }
    private:
        std::string m_Name;
        std::shared_ptr < ShaderLibrary> m_ShaderLibrary;

        std::shared_ptr <Camera> m_camera;
        float lastX = 0.0, lastY = 0.0;
        bool m_firstMouse = true;
        SceneData m_Scene;
        PointLight light;

        bool IsWireFrame = 0;
        int model = 1;

        std::shared_ptr <VertexArray> m_CubeVertexArray;
        std::shared_ptr <VertexArray> m_BunnyVertexArray;
        std::shared_ptr <VertexArray> m_DeerVertexArray;

        bool OnWindowResize(WindowResizeEvent& e) {


            m_camera->UpdateProjectionMatrix(45.0, float(e.GetWidth()) / float(e.GetHeight()), 0.1f, 1000.0f);
            return false;
        }



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

