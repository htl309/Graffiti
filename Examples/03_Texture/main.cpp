
#include<Graffiti.h>

#include"TextureLayer.h"

namespace Graffiti {
   
    class MyApplication :public Application {
    public:
        MyApplication() {

            PushLayer(new TextureLayer());
        }
        ~MyApplication() {
        }
    };

    Application* CreateApplication() {
        return new MyApplication();
    }


}
#define GF_PLATFORM_WINDOWS 

#define GF_RENDERAPI_VULKAN
#define GF_RENDERAPI_OPENGL

#include"Graffiti/EntryPoint.h"

