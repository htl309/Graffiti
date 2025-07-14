
#include<Graffiti.h>

#include"MeshShader.h"

namespace Graffiti {

    class MyApplication :public Application {
    public:
        MyApplication() {

            PushLayer(new MeshShaderLayer());
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
//#define GF_RENDERAPI_OPENGL

#include"Graffiti/EntryPoint.h"

