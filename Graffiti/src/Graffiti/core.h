#pragma once


#ifdef GF_PLATFORM_WINDOWS
	#ifdef GF_BUILD_DLL
		#define GRAFFITI_API __declspec(dllexport)
	#else
		#define GRAFFITI_API __declspec(dllimport)
	#endif
#else
	#error Graffiti only support Windows!
#endif 
