#include"gfpch.h"
#include "WindowsWindow.h"
#include "Graffiti/Events/ApplicationEvent.h"
#include "Graffiti/Events/MouseEvent.h"
#include "Graffiti/Events/KeyEvent.h"
#include "Graffiti/Render/Renderer.h"
//为了禁用输入法引入的文件
#define GLFW_EXPOSE_NATIVE_WIN32 
#include <GLFW/glfw3native.h>  // 必须包含这个才能使用 glfwGetWin32Window
#include <windows.h>

namespace Graffiti {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description) {
		GF_CORE_ERROR("GLFW　Error {0}: {1}", error, description);
	}
    std::unique_ptr<Window> Window::Create(const WindowProps& props) {
        return std::move(std::make_unique<WindowsWindow>(props));
	}
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}


	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	void WindowsWindow::Init(const  WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		

		GF_CORE_INFO("Creating window in Windows {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			GF_CORE_ASSERT(success, "Could not Init GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
		
		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // OpenGL 4.x
			//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core 模式
		}else if(Render::GetRenderAPI()== RenderAPI::API::Vulkan)  
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(int(props.Width), int(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
		
		glfwSetWindowUserPointer(m_Window, &m_Data);

		

		//设置GLFW的回调函数
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window,int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			data.framebufferResized = true;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);

		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}

		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(codepoint);
			data.EventCallback(event); 
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			
			}

		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent  event((float)xoffset,(float)yoffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent  event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});

		// 禁用输入法
		HWND hwnd = glfwGetWin32Window(m_Window);
		ImmAssociateContext(hwnd, NULL);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::BeginFrame()
	{
	}

	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
	}
	void WindowsWindow::SetVSync(bool enabled) {

		if (enabled) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const{
		return  m_Data.VSync;
	}
	void WindowsWindow::WaitForEvents() const
	{
		glfwWaitEvents();
	}
	bool WindowsWindow::IsWindowResized() const
	{
		return m_Data.framebufferResized;
	}
	void WindowsWindow::ResetWindowResizedFlag()
	{
		m_Data.framebufferResized = false;
	}
}