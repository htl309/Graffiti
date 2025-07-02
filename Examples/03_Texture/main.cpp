
#include<Graffiti.h>

#include"TextureLayer.h"

namespace Graffiti {
   
    class BunnyApplication :public Application {
    public:
        BunnyApplication() {

            PushLayer(new TextureLayer());
        }
        ~BunnyApplication() {
        }
    };

    Application* CreateApplication() {
        return new BunnyApplication();
    }


}
#define GF_PLATFORM_WINDOWS 

#define GF_RENDERAPI_VULKAN
//#define GF_RENDERAPI_OPENGL

#include"Graffiti/EntryPoint.h"

