#pragma once


#ifdef GF_PLATFORM_WINDOWS

	extern Graffiti::Application* Graffiti::CreateApplication();
	int main(int argc, char** argv) {
		
		Graffiti::Log::Init();
		
		GF_CORE_WARN("Initialized Log!");
		GF_INFO("Hello");
		auto app = Graffiti::CreateApplication();
		app->run();
		delete app;
	}

#endif 
