#pragma once

#include"Graffiti/Render/GraphicsContext.h"

//前向声明，先声明了，之后传入具体的指针
struct GLFWwindow;
namespace Graffiti {
	

	class OpenglContext:public GraphicsContext
	{
	public:
		OpenglContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}