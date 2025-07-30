#include"MeshShader.h"


namespace Graffiti {

    MeshShaderLayer::MeshShaderLayer() :Layer("05_MeshShader"), m_Name("05_MeshShader") {

        m_ShaderLibrary = std::make_shared<ShaderLibrary>();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model> deer;
        Model::LoadGLTFModel("Models/deer.gltf", deer);
        m_DeerVertexArray = VertexArray::Create();
        m_DeerVertexArray->m_Model = deer;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model> Bunny;
        Model::LoadGLTFModel("Models/bunny.gltf", Bunny);
        m_BunnyVertexArray = VertexArray::Create();
        m_BunnyVertexArray->m_Model = Bunny;

        

        meshDeer.createMeshLets(m_DeerVertexArray);
        meshBunny.createMeshLets(m_BunnyVertexArray);

        Deervertices.resize(m_DeerVertexArray->m_Model->m_Vertices.size());
        for (int i = 0; i < m_DeerVertexArray->m_Model->m_Vertices.size(); ++i) {
            Deervertices[i] = glm::vec4(m_DeerVertexArray->m_Model->m_Vertices[i].position, 1.0);
        }
        Bunnyvertices.resize(m_BunnyVertexArray->m_Model->m_Vertices.size());
        for (int i = 0; i < m_BunnyVertexArray->m_Model->m_Vertices.size(); ++i) {
            Bunnyvertices[i] = glm::vec4(m_BunnyVertexArray->m_Model->m_Vertices[i].position, 1.0);
        }

        auto shader = Shader::Create(m_Name, {
                      {ShaderType::MeshShader, "05_MeshShader/MeshShader.mesh"},
                      {ShaderType::FragmentShader, "05_MeshShader/MeshShader.frag"}
            }
        );
  
        if(Render::GetRenderAPI() == RenderAPI::API::Vulkan)
        {
            std::shared_ptr<PipelineConfigInfo> MeshShader_configInfo1 = std::make_shared<PipelineConfigInfo>(PipelineState::MeshShaderPipeLine);
            std::shared_ptr<PipelineConfigInfo> MeshShader_configInfo2 = std::make_shared<PipelineConfigInfo>(PipelineState::WireFrame | PipelineState::MeshShaderPipeLine);
            RenderCommand::SetMeshShader(true);

            shader->ClearPipeline();
            shader->AddPipeline(MeshShader_configInfo1);
            shader->AddPipeline(MeshShader_configInfo2);
           
        }

        shader->UploadStorageBuffer("Meshlets", sizeof(meshopt_Meshlet), meshBunny.meshletssize(), 1, 0);
        shader->UploadStorageBuffer("MeshletVertices", sizeof(uint32_t), meshBunny.meshlet_verticessize(), 1,1);
        shader->UploadStorageBuffer("MeshletTriangles", sizeof(uint8_t), meshBunny.meshlet_trianglessize(), 1, 2);
        shader->UploadStorageBuffer("Vertexes", sizeof(glm::vec4), m_BunnyVertexArray->m_Model->m_Vertices.size(), 1, 3);

        m_ShaderLibrary->Add(m_Name, shader);

        shader->SetStorageBuffer("Meshlets", meshDeer.meshlets.data());
        shader->SetStorageBuffer("MeshletVertices", meshDeer.meshlet_vertices.data());
        shader->SetStorageBuffer("MeshletTriangles", meshDeer.meshlet_triangles.data());
        shader->SetStorageBuffer("Vertexes", Deervertices.data());

        size1 = meshDeer.meshletssize();
        size2 = meshDeer.meshletssize();
        size3 = meshDeer.meshletssize();
        size4 = Deervertices.size();

        m_ShaderLibrary->Get(m_Name)->Link(); 

        m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);
  

    }
    MeshShaderLayer::~MeshShaderLayer()
    {
    }
    void MeshShaderLayer::OnImGuiRender()
    {
        ImGui::Begin(m_Name.data());
        ImGui::SetWindowPos(ImVec2(40, 200), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(420, 250), ImGuiCond_Always);

        ImGui::Checkbox("WireFrameMode", &IsWireFrame);
        ImGui::SameLine();
        if (ImGui::Button("ClearScene", ImVec2(160, 38))) {
            model = -1;
        }

        ImGui::Text("Models: ");
        ImGui::SameLine();
        if (ImGui::Button("Bunny", ImVec2(80, 35))) {
            m_CameraControl.SetTarget(m_BunnyVertexArray->m_Model->boundingbox);
            model = 0;

            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("Meshlets", meshBunny.meshlets.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("MeshletVertices", meshBunny.meshlet_vertices.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("MeshletTriangles", meshBunny.meshlet_triangles.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("Vertexes", Bunnyvertices.data());

            size1 = meshBunny.meshletssize();
            size2 = meshBunny.meshletssize();
            size3 = meshBunny.meshletssize();
            size4 = Bunnyvertices.size();
        }
        ImGui::SameLine();
        if (ImGui::Button("Deer", ImVec2(80, 35))) {
            model = 1;
            m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);

            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("Meshlets", meshDeer.meshlets.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("MeshletVertices", meshDeer.meshlet_vertices.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("MeshletTriangles", meshDeer.meshlet_triangles.data());
            m_ShaderLibrary->Get(m_Name)->SetStorageBuffer("Vertexes", Deervertices.data());

            size1 = meshDeer.meshletssize();
            size2 = meshDeer.meshletssize();
            size3 = meshDeer.meshletssize();
            size4 = Deervertices.size();
        }
        ImGui::Text("Meshlets: %d", size1);
        ImGui::Text("MeshletVertices: %d ", size2);
        ImGui::Text("MeshletTriangles: %d", size3);
        ImGui::Text("Vertexes: %d", size4);

        ImGui::End();
    }
    void MeshShaderLayer::OnUpdate(TimeStep& ts)
    {
        if (!ImGui::GetIO().WantCaptureMouse) {
            // 只有在 ImGui 没有占用鼠标时，才允许相机控制逻辑生效
            m_CameraControl.KeyMovement(ts);
            m_CameraControl.MouseMovement();
        }

        m_Scene.ViewProjectionMatrix = m_CameraControl.GetViewProjectionMatrix();
        m_Scene.CameraPosition = m_CameraControl.GetCameraPosition();

        Render::BeginScene(m_Scene);
        Render::SetMode(IsWireFrame);
    
        if (model == 0)    MeshShaderSubmit(meshBunny.meshletssize(), m_ShaderLibrary->Get(m_Name));
        else if (model == 1) MeshShaderSubmit(meshDeer.meshletssize(), m_ShaderLibrary->Get(m_Name));
 

        Render::EndScene();
    }
    void MeshShaderLayer::OnEvent(Event& event)
    {
        m_CameraControl.OnEvent(event);
    }
  
    void MeshShaderLayer::MeshShaderSubmit(uint32_t taskcount, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
    {
        shader->Bind(); 

       shader->SetSceneData(m_Scene);
        shader->SetTransform(transform);      
        RenderCommand::MeshShaderDraw(taskcount); 
    }

    
}