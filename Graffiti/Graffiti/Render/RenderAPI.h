#pragma once
namespace Graffiti {

	enum class RenderAPI {
		Opengl = 0,
		Vulkan =1
	};
	class Render {
	public:
		inline static RenderAPI GetRenderAPI() { return s_RenderAPI; }

	private:
		static RenderAPI s_RenderAPI;

	};
}