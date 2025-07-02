#pragma once
#include "gfpch.h"
#include "Core.h"

#include"Graffiti/Events/Event.h"
namespace Graffiti {
	
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Graffiti Engine",
			uint32_t width = 1500,
			uint32_t height = 1300)
			: Title(title), Width(width), Height(height)
		{
		}
	};
	class  Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {};


		virtual void BeginFrame() = 0;
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void WaitForEvents() const = 0;
		virtual bool IsWindowResized()const = 0;
		virtual void ResetWindowResizedFlag() = 0;


		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

		virtual void* GetNativeWindow() const = 0;

	
	
	};

	
}