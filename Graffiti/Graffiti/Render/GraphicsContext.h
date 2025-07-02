#pragma once
#include "Graffiti/base/Window.h"
namespace Graffiti {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void BeginFrame() = 0;
		virtual void BeginSwapChainRenderPass() = 0;
		virtual void SwapBuffers() = 0;
		virtual void EndFrame() = 0;

	public:
		static std::shared_ptr<GraphicsContext> Create(std::unique_ptr<Window>  window);

        std::unique_ptr<Window>  m_WindowHandle;

        static std::vector< std::shared_ptr<GraphicsContext>> G_Context;

		static std::string PhysicalGPU_Name;
		static std::string GraphicAPI_Version;

	};


}

