
#define GF_PLATFORM_WINDOWS

#include<Graffiti.h>

class ExampleLayer : public Graffiti::Layer {
public:
	ExampleLayer():Layer("Example") {

	}
	void OnUpdate() override {
		
			
	}

	void OnEvent(Graffiti::Event& event) override {
		if (event.GetEventType() == Graffiti::EventType::KeyPressed)
		{
			Graffiti::KeyPressedEvent& e = (Graffiti::KeyPressedEvent&)event;
			if (e.GetKeyCode() == GF_KEY_TAB)
				GF_TRACE("Tab key is pressed (event)!");
			GF_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};
class Sanbox :public Graffiti::Application {
	public:
		Sanbox() {
			
			PushLayer(new ExampleLayer());
			
			
		}
		~Sanbox() {

		}
};

Graffiti::Application* Graffiti::CreateApplication() {
	return new Sanbox();
}

