#pragma once
#include "gfpch.h"
#include "Core.h"

#include"Events/Event.h"
namespace Graffiti {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Graffiti Engine",
			uint32_t width = 800,
			uint32_t height = 600)
			: Title(title), Width(width), Height(height)
		{
		}
	};
	class GRAFFITI_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {};
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		virtual void* GetNativeWindow() const = 0;
	};
}