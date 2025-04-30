#pragma once

#include"Graffiti/Layer.h"
#include"Graffiti/Events/MouseEvent.h"
#include"Graffiti/Events/KeyEvent.h"
#include"Graffiti/Events/ApplicationEvent.h"
namespace Graffiti {
	class GRAFFITI_API ImGuiLayer:public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		
		virtual void OnImGuiRender();

		void Begin();
		void end();
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