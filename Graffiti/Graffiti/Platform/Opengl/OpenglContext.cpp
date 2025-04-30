#include "gfpch.h"
#include "OpenglContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Graffiti {
	OpenglContext::OpenglContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
	}
	void OpenglContext::Init()
	{

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GF_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		GF_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR))); 
		GF_CORE_INFO("GPU: {0}",reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		GF_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}
	void OpenglContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}