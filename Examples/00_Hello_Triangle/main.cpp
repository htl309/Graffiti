
#include<Graffiti.h>


namespace Graffiti {
	class Hello_Triangle : public Layer {
	public:
		Hello_Triangle() :Layer("00_Hello_Triangle"), m_Name("00_Hello_Triangle"){
			m_VertexArray = VertexArray::Create();
			m_ShaderLibrary = std::make_shared<ShaderLibrary>();
			m_camera = std::make_shared<Camera>(glm::vec3{ 0.0,0.0,2.0 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 }, 45.0, 15.0/ 13.0, 0.1, 10);

			std::vector<Vertex> vertices(3);
			vertices[0].position = { -0.5,-0.5, 0.0 };
			vertices[1].position = {  0.5,-0.5, 0.0 }; 
			vertices[2].position = {  0.0, 0.4, 0.0 };
            std::vector<uint32_t> indices = {0, 1, 2,};


			m_VertexBuffer = VertexBuffer::Create(vertices, vertices.size());
			/*BufferLayout layout  = {
				{ ShaderDataType::Float3,"inposition"},
				{ ShaderDataType::Float3,"innormal"},
				{ ShaderDataType::Float2,"intexCord" }
			};
			m_VertexBuffer->SetLayout(layout);*/
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			
			m_IndexBuffer = IndexBuffer::Create(indices, indices.size());
			m_VertexArray->AddIndexBuffer(m_IndexBuffer);

			auto shader = Shader::Create(m_Name);  
			shader->Load(ShaderType::VertexShader, "00_Hello_Triangle/Hello_Triangle.vert");
			shader->Load(ShaderType::FragmentShader, "00_Hello_Triangle/Hello_Triangle.frag");


			m_ShaderLibrary->Add(m_Name, shader);


			color.resize(4);
			color[0] = { 1.0,0.0,0.0,1.0 };
			color[1] = { 0.0,1.0,0.0,1.0 };
			color[2] = { 0.0,0.0,1.0,1.0 };
			color[3] = { 0.0,0.0,1.0,1.0 };
  
  
			m_ShaderLibrary->Get(m_Name)->UploadUniformBuffer("color",sizeof(glm::vec4), 3, 1, 0);  
			m_ShaderLibrary->Get(m_Name)->Link();   
			
		}

		void OnImGuiRender() override {
			ImGui::Begin(m_Name.data());
			ImGui::SetWindowPos(ImVec2(40, 200), ImGuiCond_Always);
			ImGui::SetWindowSize(ImVec2(420, 180), ImGuiCond_Always);
			ImGui::ColorEdit3("Vertex1", glm::value_ptr(color[0]));
			ImGui::ColorEdit3("Vertex2", glm::value_ptr(color[1]));
			ImGui::ColorEdit3("Vertex3", glm::value_ptr(color[2]));
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


			m_Scene.ViewProjectionMatrix = m_camera->GetViewProjectionMatrix();
         /*   GF_CORE_INFO("{0},{1},{2}", m_Scene.ViewProjectionMatrix[0][0],
                                                                    m_Scene.ViewProjectionMatrix[1][1], 
                                                                    m_Scene.ViewProjectionMatrix[2][2]);*/
			Render::BeginScene(m_Scene);
     
            m_ShaderLibrary->Get(m_Name)->SetUniformBuffer("color", color.data());      
			Render::Submit(m_VertexArray, m_ShaderLibrary->Get(m_Name));      
            
			Render::EndScene();
		}

		void OnEvent(Event& event) override {

			EventDispatcher dispatcher(event);
			dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(Hello_Triangle::OnWindowResize));
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
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<VertexBuffer>  m_VertexBuffer;
		std::shared_ptr <VertexArray> m_VertexArray;
	
	
		std::shared_ptr <Camera> m_camera;
		SceneData m_Scene;


		bool OnWindowResize(WindowResizeEvent& e) {

  
			m_camera->UpdateProjectionMatrix(45.0, float(e.GetWidth()) / float(e.GetHeight()), 0.1f, 10000.0f);
			return false;
		}

		std::vector<glm::vec4> color; 
		
	};


	class Triangle :public Application {
	public:
		Triangle() {

			PushLayer(new Hello_Triangle());
		}
		~Triangle() {
		}
	};

	Application* CreateApplication() {
		return new Triangle();
	}


}
#define GF_PLATFORM_WINDOWS 

#define GF_RENDERAPI_VULKAN

//#define GF_RENDERAPI_OPENGL


#include"Graffiti/EntryPoint.h"