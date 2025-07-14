#include "gfpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>


namespace Graffiti {
	OpenGLContext::OpenGLContext(std::unique_ptr<Window> windowHandle)
	{
        m_WindowHandle = std::move(windowHandle);
	}
	void OpenGLContext::Init()
	{
       
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_WindowHandle->GetNativeWindow()));
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        m_WindowHandle->SetVSync(1);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GF_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		PhysicalGPU_Name = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		int major = 0, minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		GraphicAPI_Version = "OpenGl" + std::to_string(major)+"." + std::to_string(minor);
		
	}
	void OpenGLContext::BeginFrame()
	{
	}
	void OpenGLContext::BeginSwapChainRenderPass()
	{
	}
	void OpenGLContext::SwapBuffers()
	{
        m_WindowHandle->OnUpdate();
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_WindowHandle->GetNativeWindow()));
	}
	void OpenGLContext::EndFrame()
	{
	}
}