#pragma once

#include"Graffiti/Render/GraphicsContext.h"

//ǰ���������������ˣ�֮��������ָ��
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