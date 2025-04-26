#pragma once
#include"core.h"
namespace Graffiti {
	class GRAFFITI_API Application
	{
	public:
		Application();
		virtual ~Application();
		void run();
	};

	Application* CreateApplication();

}

