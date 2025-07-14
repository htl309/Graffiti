#include"MeshShader.h"


namespace Graffiti {

    MeshShaderLayer::MeshShaderLayer() :Layer("05_MeshShader"), m_Name("05_MeshShader") {

        m_ShaderLibrary = std::make_shared<ShaderLibrary>();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model> deer;
        Model::LoadGLTFModel("Models/deer.gltf", deer);
        m_DeerVertexArray = VertexArray::Create();
        m_DeerVertexArray->m_Model = deer;
       //m_DeerVertexArray->AddModel(deer);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<Model> bunny;
        Model::LoadGLTFModel("Models/bunny.gltf", bunny);
        m_BunnyVertexArray = VertexArray::Create();
        m_BunnyVertexArray->m_Model = bunny;
      //m_BunnyVertexArray->AddModel(bunny);
        

        meshDeer.createMeshLets(m_DeerVertexArray);
        meshBunny.createMeshLets(m_BunnyVertexArray);

        auto shader = Shader::Create(m_Name, {
                  {ShaderType::MeshShader, "05_MeshShader/test.mesh"},
                  {ShaderType::FragmentShader, "05_MeshShader/test.frag"}
                     //{ShaderType::MeshShader, "05_MeshShader/MeshShader.mesh"},
                     //{ShaderType::FragmentShader, "05_MeshShader/MeshShader.frag"}
            }
        );

        std::shared_ptr<PipelineConfigInfo> MeshShader_configInfo = std::make_shared<PipelineConfigInfo>();
        MakePipeLine(MeshShader_configInfo, PipelineState::Common);
        //shader->ClearPipeline();
        shader->AddPipeline(MeshShader_configInfo);

        shader->UploadStorageBuffer("Meshlets", sizeof(meshopt_Meshlet), meshDeer.meshletssize(), 1, 0);
        shader->UploadStorageBuffer("MeshletVertices", sizeof(uint32_t), meshDeer.meshlet_verticessize(), 1, 1);
        shader->UploadStorageBuffer("MeshletTriangles", sizeof(uint8_t), meshDeer.meshlet_trianglessize(), 1, 2);
        shader->UploadStorageBuffer("Vertexes", sizeof(Vertex), m_DeerVertexArray->m_Model->m_Vertices.size(), 1, 3);
     
        shader->SetStorageBuffer("Meshlets", meshDeer.meshlets.data());
        shader->SetStorageBuffer("MeshletVertices", meshDeer.meshlet_vertices.data());
        shader->SetStorageBuffer("MeshletTriangles", meshDeer.meshlet_triangles.data());
        shader->SetStorageBuffer("Vertexes", m_DeerVertexArray->m_Model->m_Vertices.data());
       
        m_ShaderLibrary->Add(m_Name, shader); 
        m_ShaderLibrary->Get(m_Name)->Link(); 

   //  m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);
    }
    MeshShaderLayer::~MeshShaderLayer()
    {
    }
    void MeshShaderLayer::OnImGuiRender()
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
        if (ImGui::Button("Bunny", ImVec2(80, 35))) {
            model = 0;
            RenderCommand::SetDepthtest(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Deer", ImVec2(80, 35))) {
            model = 1;
            m_CameraControl.SetTarget(m_DeerVertexArray->m_Model->boundingbox);
            RenderCommand::SetDepthtest(true);
        }


        ImGui::End();
    }
    void MeshShaderLayer::OnUpdate(TimeStep& ts)
    {
        if (!ImGui::GetIO().WantCaptureMouse) {
            // 只有在 ImGui 没有占用鼠标时，才允许相机控制逻辑生效
            m_CameraControl.KeyMovement(ts);
            m_CameraControl.MouseMovement(ts);
        }
    
        m_Scene.ViewProjectionMatrix = glm::mat4(1.0);// m_CameraControl.GetViewProjectionMatrix();
        m_Scene.CameraPosition = glm::vec3(1.0);

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

    void MeshShaderLayer::MakePipeLine(std::shared_ptr<PipelineConfigInfo> MeshShader_configInfo, PipelineState state)
    {
        MeshShader_configInfo->m_State = state;
        MeshShader_configInfo->viewportInfo = new VkPipelineViewportStateCreateInfo();
        MeshShader_configInfo->inputAssemblyInfo = new VkPipelineInputAssemblyStateCreateInfo();
        MeshShader_configInfo->rasterizationInfo = new VkPipelineRasterizationStateCreateInfo();
        MeshShader_configInfo->multisampleInfo = new VkPipelineMultisampleStateCreateInfo();
        MeshShader_configInfo->colorBlendAttachment = new VkPipelineColorBlendAttachmentState();
        MeshShader_configInfo->colorBlendInfo = new VkPipelineColorBlendStateCreateInfo();
        MeshShader_configInfo->depthStencilInfo = new VkPipelineDepthStencilStateCreateInfo();
        MeshShader_configInfo->dynamicStateInfo = new VkPipelineDynamicStateCreateInfo();

        //MeshShader_configInfo->inputAssemblyInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        //MeshShader_configInfo->inputAssemblyInfo->topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        //MeshShader_configInfo->inputAssemblyInfo->primitiveRestartEnable = VK_FALSE;

        MeshShader_configInfo->viewportInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        MeshShader_configInfo->viewportInfo->viewportCount = 1;
        MeshShader_configInfo->viewportInfo->pViewports = nullptr;
        MeshShader_configInfo->viewportInfo->scissorCount = 1;
        MeshShader_configInfo->viewportInfo->pScissors = nullptr;

        MeshShader_configInfo->rasterizationInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        MeshShader_configInfo->rasterizationInfo->depthClampEnable = VK_FALSE;
        MeshShader_configInfo->rasterizationInfo->rasterizerDiscardEnable = VK_FALSE;

        MeshShader_configInfo->rasterizationInfo->polygonMode = HasState(state, PipelineState::WireFrame) ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
        MeshShader_configInfo->rasterizationInfo->lineWidth = 1.0f;
        MeshShader_configInfo->rasterizationInfo->cullMode = VK_CULL_MODE_NONE;
        MeshShader_configInfo->rasterizationInfo->frontFace = VK_FRONT_FACE_CLOCKWISE;
        MeshShader_configInfo->rasterizationInfo->depthBiasEnable = VK_FALSE;
        MeshShader_configInfo->rasterizationInfo->depthBiasConstantFactor = 0.0f;  // Optional
        MeshShader_configInfo->rasterizationInfo->depthBiasClamp = 0.0f;           // Optional
        MeshShader_configInfo->rasterizationInfo->depthBiasSlopeFactor = 0.0f;     // Optional

        MeshShader_configInfo->multisampleInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        MeshShader_configInfo->multisampleInfo->sampleShadingEnable = VK_FALSE;
        MeshShader_configInfo->multisampleInfo->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        MeshShader_configInfo->multisampleInfo->minSampleShading = 1.0f;           // Optional
        MeshShader_configInfo->multisampleInfo->pSampleMask = nullptr;             // Optional
        MeshShader_configInfo->multisampleInfo->alphaToCoverageEnable = VK_FALSE;  // Optional
        MeshShader_configInfo->multisampleInfo->alphaToOneEnable = VK_FALSE;       // Optional

        MeshShader_configInfo->colorBlendAttachment->colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;


        MeshShader_configInfo->colorBlendAttachment->blendEnable = VK_TRUE;
        MeshShader_configInfo->colorBlendAttachment->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        MeshShader_configInfo->colorBlendAttachment->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        MeshShader_configInfo->colorBlendAttachment->colorBlendOp = VK_BLEND_OP_ADD;
        MeshShader_configInfo->colorBlendAttachment->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        MeshShader_configInfo->colorBlendAttachment->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        MeshShader_configInfo->colorBlendAttachment->alphaBlendOp = VK_BLEND_OP_ADD;

        MeshShader_configInfo->colorBlendInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        MeshShader_configInfo->colorBlendInfo->logicOpEnable = VK_FALSE;
        MeshShader_configInfo->colorBlendInfo->logicOp = VK_LOGIC_OP_COPY;  // Optional
        MeshShader_configInfo->colorBlendInfo->attachmentCount = 1;
        MeshShader_configInfo->colorBlendInfo->pAttachments = MeshShader_configInfo->colorBlendAttachment;
        MeshShader_configInfo->colorBlendInfo->blendConstants[0] = 0.0f;  // Optional
        MeshShader_configInfo->colorBlendInfo->blendConstants[1] = 0.0f;  // Optional
        MeshShader_configInfo->colorBlendInfo->blendConstants[2] = 0.0f;  // Optional
        MeshShader_configInfo->colorBlendInfo->blendConstants[3] = 0.0f;  // Optional

        MeshShader_configInfo->depthStencilInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        MeshShader_configInfo->depthStencilInfo->depthTestEnable = VK_TRUE;
        MeshShader_configInfo->depthStencilInfo->depthWriteEnable = HasState(state, PipelineState::DepthTestDisable) ? VK_FALSE : VK_TRUE;
        MeshShader_configInfo->depthStencilInfo->depthCompareOp = VK_COMPARE_OP_LESS;
        MeshShader_configInfo->depthStencilInfo->depthBoundsTestEnable = VK_FALSE;
        MeshShader_configInfo->depthStencilInfo->minDepthBounds = 0.0f;  // Optional
        MeshShader_configInfo->depthStencilInfo->maxDepthBounds = 1.0f;  // Optional
        MeshShader_configInfo->depthStencilInfo->stencilTestEnable = VK_FALSE;
        MeshShader_configInfo->depthStencilInfo->front = {};  // Optional
        MeshShader_configInfo->depthStencilInfo->back = {};   // Optional

        MeshShader_configInfo->dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        MeshShader_configInfo->dynamicStateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        MeshShader_configInfo->dynamicStateInfo->pDynamicStates = MeshShader_configInfo->dynamicStateEnables.data();
        MeshShader_configInfo->dynamicStateInfo->dynamicStateCount =
            static_cast<uint32_t>(MeshShader_configInfo->dynamicStateEnables.size());
        MeshShader_configInfo->dynamicStateInfo->flags = 0;

    }
}