
#include"gfpch.h"

#include "Application.h"
#include "Log.h"

#include"Graffiti/Render/Renderer.h"
#include"Graffiti/Render/RenderCommand.h"

#include"GLFW/glfw3.h"
#include "Graffiti/ImGui/imgui.h"
#include "Graffiti/Graphic/OpenGL/OpenGLImGuiLayer.h"

namespace Graffiti {

	Application* Application::s_Instance = nullptr;
	
	
	Application::Application(){
    
 
		GF_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		//创建窗口
        std::unique_ptr<Window> m_window=Window::Create() ;
        m_window->SetEventCallback(GF_BIND_EVENT_FN(Application::OnEvent));
		
		
        //创建上下文
        std::shared_ptr<GraphicsContext>  m_Context = GraphicsContext::Create(std::move(m_window));
		m_Context->Init();
        GraphicsContext::G_Context.push_back(std::move(m_Context));
        m_ContextIndex = GraphicsContext::G_Context.size() - 1;

        //绑定渲染命令
		Render::Init(m_ContextIndex);

        //添加ImGuiLayer
		m_ImGuiLayer = ImGuiLayer::CreateImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application() {

	}

	void Application::run() {
		
		
		while (m_Running) {
           
            GraphicsContext::G_Context[m_ContextIndex]->BeginFrame(); 
			GraphicsContext::G_Context[m_ContextIndex]->BeginSwapChainRenderPass(); 
			float time = (float)glfwGetTime();
			m_TimeStep = time - m_LastTime;
			m_LastTime = time;
			
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(m_TimeStep);
				} 
				m_ImGuiLayer->Begin(); 
				for (Layer* layer : m_LayerStack) {
					layer->OnImGuiRender(); 
				}
				m_ImGuiLayer->End();
				/*auto [x, y] = Input::GetMousePostion();
				GF_CORE_TRACE("{0},{1}", x, y);*/
			} 
            
            GraphicsContext::G_Context[m_ContextIndex]->SwapBuffers();  
			GraphicsContext::G_Context[m_ContextIndex]->EndFrame();  
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
    void* Application::GetWindow()
    {
      return  GraphicsContext::G_Context[m_ContextIndex]->m_WindowHandle->GetNativeWindow();
    }
    uint32_t Application::GetWindowWidth()
    {
        return  GraphicsContext::G_Context[m_ContextIndex]->m_WindowHandle->GetWidth();
    }
    uint32_t Application::GetWindowHeight()
    {
        return  GraphicsContext::G_Context[m_ContextIndex]->m_WindowHandle->GetHeight();
    }
	void Application::OnEvent(Event& e) {
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch< WindowCloseEvent >(GF_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(Application::OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		

		m_Minimized = false;
		Render::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
	
}