#pragma once
#include"gfpch.h"
#include"Graffiti/base/Window.h"
#include"GLFW/glfw3.h"
#include"Graffiti/Render/GraphicsContext.h"
namespace Graffiti {

	class  WindowsWindow:public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();


		void BeginFrame() override;
		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void WaitForEvents() const override;
		virtual bool IsWindowResized()const override;
		virtual void ResetWindowResizedFlag() override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const  WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
			//这个变量是判断窗口大小有没有发生改变
			bool framebufferResized = false;
		};

		WindowData m_Data;
	};
}


