#pragma once

#include "Graffiti/base/Core.h"
#include "Graffiti/base/Window.h"
#include "Graffiti/base/LayerStack.h"
#include "Graffiti/base/ImGuiLayer.h"
#include "Graffiti/Events/Event.h"
#include "Graffiti/Events/ApplicationEvent.h"
#include "Graffiti/Render/GraphicsContext.h"


#include"Graffiti/base/Input.h"

#include"Graffiti/base/TimeStep.h"

namespace Graffiti {


	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);



		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void* GetWindow();
        uint32_t GetWindowWidth();
        uint32_t GetWindowHeight();
		static Application& Get() { return *s_Instance; }
	private:
		
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:

		bool m_Running = true;
		bool m_Minimized = false;

		uint32_t m_ContextIndex;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;


		float m_LastTime = 0;
		TimeStep m_TimeStep;
		
	private:
		static Application* s_Instance;

	};

	Application* CreateApplication();

}

