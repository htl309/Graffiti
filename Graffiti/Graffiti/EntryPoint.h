#pragma once

#ifdef GF_RENDERAPI_OPENGL
	Graffiti::RenderAPI::API Graffiti::RenderAPI::s_API = Graffiti::RenderAPI::API::OpenGL;
#elif  defined(GF_RENDERAPI_VULKAN)
	Graffiti::RenderAPI::API Graffiti::RenderAPI::s_API = Graffiti::RenderAPI::API::Vulkan;
#endif
	
#ifdef GF_PLATFORM_WINDOWS

	extern Graffiti::Application* Graffiti::CreateApplication();
	int main(int argc, char** argv) {
		
		Graffiti::Log::Init();
		GF_INFO("Graffiti Engine Starting");

		auto app = Graffiti::CreateApplication();    
		app->run();     
		delete app; 

        return 0;
	}

#endif 
