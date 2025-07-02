#include"TextureLayer.h"

namespace Graffiti {

    void createCube(std::shared_ptr<Model>  cube) {
        std::vector<Vertex>& vertices = cube->m_Vertices;
        std::vector<uint32_t>& indices = cube->m_Indices;
        vertices = {
            // 前面 (+Z)
            {{-1.5f, -1.5f,  1.5f}, { 0,  0,  1}, {0.0f, 0.0f}},
            {{ 1.5f, -1.5f,  1.5f}, { 0,  0,  1}, {1.0f, 0.0f}},
            {{ 1.5f,  1.5f,  1.5f}, { 0,  0,  1}, {1.0f, 1.0f}},
            {{-1.5f, -1.5f,  1.5f}, { 0,  0,  1}, {0.0f, 0.0f}},
            {{ 1.5f,  1.5f,  1.5f}, { 0,  0,  1}, {1.0f, 1.0f}},
            {{-1.5f,  1.5f,  1.5f}, { 0,  0,  1}, {0.0f, 1.0f}},

            // 后面 (-Z)
            {{ 1.5f, -1.5f, -1.5f}, { 0,  0, -1}, {0.0f, 0.0f}},
            {{-1.5f, -1.5f, -1.5f}, { 0,  0, -1}, {1.0f, 0.0f}},
            {{-1.5f,  1.5f, -1.5f}, { 0,  0, -1}, {1.0f, 1.0f}},
            {{ 1.5f, -1.5f, -1.5f}, { 0,  0, -1}, {0.0f, 0.0f}},
            {{-1.5f,  1.5f, -1.5f}, { 0,  0, -1}, {1.0f, 1.0f}},
            {{ 1.5f,  1.5f, -1.5f}, { 0,  0, -1}, {0.0f, 1.0f}},

            // 左面 (-X)
            {{-1.5f, -1.5f, -1.5f}, {-1,  0,  0}, {0.0f, 0.0f}},
            {{-1.5f, -1.5f,  1.5f}, {-1,  0,  0}, {1.0f, 0.0f}},
            {{-1.5f,  1.5f,  1.5f}, {-1,  0,  0}, {1.0f, 1.0f}},
            {{-1.5f, -1.5f, -1.5f}, {-1,  0,  0}, {0.0f, 0.0f}},
            {{-1.5f,  1.5f,  1.5f}, {-1,  0,  0}, {1.0f, 1.0f}},
            {{-1.5f,  1.5f, -1.5f}, {-1,  0,  0}, {0.0f, 1.0f}},

            // 右面 (+X)
            {{ 1.5f, -1.5f,  1.5f}, { 1,  0,  0}, {0.0f, 0.0f}},
            {{ 1.5f, -1.5f, -1.5f}, { 1,  0,  0}, {1.0f, 0.0f}},
            {{ 1.5f,  1.5f, -1.5f}, { 1,  0,  0}, {1.0f, 1.0f}},
            {{ 1.5f, -1.5f,  1.5f}, { 1,  0,  0}, {0.0f, 0.0f}},
            {{ 1.5f,  1.5f, -1.5f}, { 1,  0,  0}, {1.0f, 1.0f}},
            {{ 1.5f,  1.5f,  1.5f}, { 1,  0,  0}, {0.0f, 1.0f}},

            // 顶面 (+Y)
            {{-1.5f,  1.5f,  1.5f}, { 0,  1,  0}, {0.0f, 0.0f}},
            {{ 1.5f,  1.5f,  1.5f}, { 0,  1,  0}, {1.0f, 0.0f}},
            {{ 1.5f,  1.5f, -1.5f}, { 0,  1,  0}, {1.0f, 1.0f}},
            {{-1.5f,  1.5f,  1.5f}, { 0,  1,  0}, {0.0f, 0.0f}},
            {{ 1.5f,  1.5f, -1.5f}, { 0,  1,  0}, {1.0f, 1.0f}},
            {{-1.5f,  1.5f, -1.5f}, { 0,  1,  0}, {0.0f, 1.0f}},

            // 底面 (-Y)
            {{-1.5f, -1.5f, -1.5f}, { 0, -1,  0}, {0.0f, 0.0f}},
            {{ 1.5f, -1.5f, -1.5f}, { 0, -1,  0}, {1.0f, 0.0f}},
            {{ 1.5f, -1.5f,  1.5f}, { 0, -1,  0}, {1.0f, 1.0f}},
            {{-1.5f, -1.5f, -1.5f}, { 0, -1,  0}, {0.0f, 0.0f}},
            {{ 1.5f, -1.5f,  1.5f}, { 0, -1,  0}, {1.0f, 1.0f}},
            {{-1.5f, -1.5f,  1.5f}, { 0, -1,  0}, {0.0f, 1.0f}},
        };
        for (int i = 0; i < 36; i++)indices.push_back(i);

        cube->boundingbox.maxPos = glm::vec3(1.5, 1.5, 1.5);
        cube->boundingbox.minPos = glm::vec3(-1.5, -1.5, -1.5);
    }
    TextureLayer::TextureLayer() :Layer("03_Texture"), m_Name("03_Texture") {

        m_ShaderLibrary = std::make_shared<ShaderLibrary>();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model> deer;
      
        Model::LoadGLTFModel("Models/deer.gltf", deer, m_Texture_deer);

        m_DeerVertexArray = VertexArray::Create();
        m_DeerVertexArray->AddModel(deer);

        //立方体模型加载//////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model>  cube = std::make_shared<Model>();
        createCube(cube);
        m_CubeVertexArray = VertexArray::Create();
        m_CubeVertexArray->AddModel(cube);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        m_Texture_Graffiti0 = Texture::Create("resource\\logo\\Graffiti_LOGO2.png");
        m_Texture_Graffiti1 = Texture::Create("resource\\logo\\LOGO_INVERT2.png");
        m_Texture = Texture::Create("Models\\Square.png");

        auto shader = Shader::Create(m_Name, {
                 {ShaderType::VertexShader, "03_Texture/Texture.vert"},
                 {ShaderType::FragmentShader, "03_Texture/Texture.frag"}
            }
        );

        std::shared_ptr<PipelineConfigInfo> configInfo1 = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame);
        std::shared_ptr<PipelineConfigInfo> configInfo2 = std::make_shared<PipelineConfigInfo>(PipelineState::DepthTestDisable);
        std::shared_ptr<PipelineConfigInfo> configInfo3 = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame| PipelineState::DepthTestDisable);

        shader->AddPipeline(configInfo1);
        shader->AddPipeline(configInfo2);
        shader->AddPipeline(configInfo3);

        shader->UploadTexture("deer", m_Texture_deer, 1, 0); 
        m_ShaderLibrary->Add(m_Name, shader); 
        m_ShaderLibrary->Get(m_Name)->Link(); 

        m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);

        Texture_Graffiti0_ID = RenderCommand::GetTextureID(m_Texture_Graffiti0);
        Texture_Graffiti1_ID = RenderCommand::GetTextureID(m_Texture_Graffiti1);
        Texture_deer_ID = RenderCommand::GetTextureID(m_Texture_deer);
        Texture_ID = RenderCommand::GetTextureID(m_Texture);
    }
    TextureLayer::~TextureLayer()
    {
    }
    void TextureLayer::OnImGuiRender()
    {
        ImGui::Begin(m_Name.data());
        ImGui::SetWindowPos(ImVec2(40, 200), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(420, 570), ImGuiCond_Always);

        ImGui::Checkbox("WireFrameMode", &IsWireFrame);
        ImGui::SameLine();
        if (ImGui::Button("ClearScene", ImVec2(160, 38))) {
            model = -1;
        }

        ImGui::Text("Models: ");
        ImGui::SameLine();
        if (ImGui::Button("Cube", ImVec2(80, 35))) {
            model = 0;
            m_CameraControl.SetTarget(m_CubeVertexArray->m_Model->boundingbox);
            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture_Graffiti1, 1, 0);
            RenderCommand::SetDepthtest(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Deer", ImVec2(80, 35))) {
            model = 1;
            m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);
            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture_deer, 1, 0);
            RenderCommand::SetDepthtest(true);
        }



 
        ImGui::Text("Textures: ");
        if (ImGui::ImageButton("Texture_Graffiti0", Texture_Graffiti0_ID, ImVec2(190, 190), ImVec2(0, 1), ImVec2(1, 0), ImVec4(0.9, 0.9, 0.9, 1))) {

            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture_Graffiti0, 1, 0);
            RenderCommand::SetDepthtest(false);
        }
        ImGui::SameLine();
        if (ImGui::ImageButton("Texture_Graffiti1", Texture_Graffiti1_ID, ImVec2(190, 190), ImVec2(0, 1), ImVec2(1, 0))) {
            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture_Graffiti1, 1, 0);
            RenderCommand::SetDepthtest(true);
        }

        if (ImGui::ImageButton("Texture_deer", Texture_deer_ID, ImVec2(190, 190), ImVec2(0, 1), ImVec2(1, 0))) {
            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture_deer, 1, 0);
            RenderCommand::SetDepthtest(true);
        }
        ImGui::SameLine();
        if (ImGui::ImageButton("Texture", Texture_ID, ImVec2(190, 190), ImVec2(0, 1), ImVec2(1, 0))) {
            m_ShaderLibrary->Get(m_Name)->SetTexture(m_Texture,1,0);
            RenderCommand::SetDepthtest(true);
        }
        ImGui::End();
    }
    void TextureLayer::OnUpdate(TimeStep& ts)
    {
        if (!ImGui::GetIO().WantCaptureMouse) {
            // 只有在 ImGui 没有占用鼠标时，才允许相机控制逻辑生效
            m_CameraControl.KeyMovement(ts);
            m_CameraControl.MouseMovement(ts);
        }

        m_Scene.ViewProjectionMatrix = m_CameraControl.GetViewProjectionMatrix();
        m_Scene.CameraPosition = m_CameraControl.GetCameraPosition();

        Render::BeginScene(m_Scene);
        Render::SetMode(IsWireFrame);

        if (model == 0)    Render::Submit(m_CubeVertexArray, m_ShaderLibrary->Get(m_Name));
        else if (model == 1) Render::Submit(m_DeerVertexArray, m_ShaderLibrary->Get(m_Name));

        Render::EndScene();
    }
    void TextureLayer::OnEvent(Event& event)
    {
        m_CameraControl.OnEvent(event);
    }
}