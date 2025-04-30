#pragma once
#include <glad/glad.h> 
#include"Core.h"
#include"Events/Event.h"
#include"LayerStack.h"
#include"Events/ApplicationEvent.h"
#include"Window.h"

#include "imGui/ImGuiLayer.h"

#include"Input.h"

#include"Graffiti/Render/Shader.h"
#include"Graffiti/Render/Buffer.h"
#include"Graffiti/Render/VertexArray.h"
namespace Graffiti {
	class GRAFFITI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);



		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }
	private:
		
		bool OnWindowClose(WindowCloseEvent& e);

	private:

		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;


		std::unique_ptr<Shader> m_Shader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr <VertexArray> m_VertexArray;
	private:
		static Application* s_Instance;

	};

	Application* CreateApplication();

}

