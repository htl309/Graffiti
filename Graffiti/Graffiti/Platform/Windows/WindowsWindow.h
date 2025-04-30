#pragma once
#include"gfpch.h"
#include"Graffiti/Window.h"
#include"GLFW/glfw3.h"
#include "Graffiti/Platform/Opengl/OpenglContext.h"
namespace Graffiti {

	class GRAFFITI_API WindowsWindow:public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();


		
		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const  WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<OpenglContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}


