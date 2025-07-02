#pragma once


#ifdef GF_PLATFORM_WINDOWS
	#ifdef GF_BUILD_DLL
		#define GRAFFITI_API __declspec(dllexport)
	#elif GF_USE_DLL
		#define GRAFFITI_API __declspec(dllimport)
	#else
		#define GRAFFITI_API 
	#endif
#else
    #define GRAFFITI_API 
	//#error Graffiti only support Windows!
#endif 


#ifdef GF_ENABLE_ASSERTS

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define GF_ASSERT(x,...) GF_EXPAND_MACRO( GF_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define GF_CORE_ASSERT(x,...) GF_EXPAND_MACRO( GF_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define GF_ASSERT(x,...)  do {                                                      \
        if (!(x)) {                                           \
			GF_ERROR(__VA_ARGS__);				      \
            assert(x);                                        \
        }                                                     \
    } while (0)
#define GF_CORE_ASSERT(x,...)do {                                                      \
        if (!(x)) {                                           \
			GF_CORE_ERROR(__VA_ARGS__);					  \
            assert(x);                                        \
        }                                                     \
    } while (0)
#endif

#define GF_BIT(x)(1<<x)

#define GF_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)


