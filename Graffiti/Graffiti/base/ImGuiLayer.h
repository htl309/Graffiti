#pragma once

#include"Graffiti/base/Layer.h"
#include"Graffiti/Events/MouseEvent.h"
#include"Graffiti/Events/KeyEvent.h"
#include"Graffiti/Events/ApplicationEvent.h"
namespace Graffiti {
	class  ImGuiLayer:public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();
	

		virtual void Begin()=0;
		virtual void End()= 0;

		
		void ImGuiLayer::OnImGuiRender() override;

		static ImGuiLayer* CreateImGuiLayer();

		void SetStyle();

	private:
		static int frameCount;
		static float fps;
		static float lastTime;
	/*	void OnEvent(Event& event) override;
	private:
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);*/
	};

}