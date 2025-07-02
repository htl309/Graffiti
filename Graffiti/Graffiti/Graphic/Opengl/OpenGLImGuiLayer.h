#pragma once

#include "Graffiti/base/ImGuiLayer.h"

namespace Graffiti {

	class OpenGLImGuiLayer: public ImGuiLayer {
	public:
		OpenGLImGuiLayer();
		~OpenGLImGuiLayer();


		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin();
		virtual void End();

		static ImTextureID GetTextureID(std::shared_ptr<Texture> Texture);
		
	};

}