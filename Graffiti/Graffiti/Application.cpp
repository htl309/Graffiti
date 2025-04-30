#include"gfpch.h"

#include "Application.h"
#include "Log.h"


namespace Graffiti {

	Application* Application::s_Instance = nullptr;


	Application::Application() {

		GF_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
			
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(GF_BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


	
		m_VertexArray = VertexArray::Create();
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VertexBuffer=VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Position"}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer=IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->AddIndexBuffer(m_IndexBuffer);
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

	}
	Application::~Application() {

	}

	void Application::run() {
		
		
		while (m_Running) {
			

			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) {
			
				layer->OnUpdate();
			}
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {

				layer->OnImGuiRender();
			}
			m_ImGuiLayer->end();
			/*auto [x, y] = Input::GetMousePostion();
			GF_CORE_TRACE("{0},{1}", x, y);*/

			m_Window->OnUpdate();
		}

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}


	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}
	void Application::OnEvent(Event& e) {
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch< WindowCloseEvent >(GF_BIND_EVENT_FN(Application::OnWindowClose));
		

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}