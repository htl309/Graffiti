#include "gfpch.h"

#include <GLFW/glfw3.h>

#include "Graffiti/ImGui/imgui.h"

#include "Graffiti/ImGui/imgui_impl_glfw.h"
#include "Graffiti/ImGui/imgui_impl_opengl3.h"

#include "Graffiti/Render/GraphicsContext.h"
#include "Graffiti/Render/Renderer.h"
#include "OpenGLImGuiLayer.h"

namespace Graffiti {
	OpenGLImGuiLayer::OpenGLImGuiLayer() :ImGuiLayer() {}

	OpenGLImGuiLayer::~OpenGLImGuiLayer() {}

	void OpenGLImGuiLayer::OnAttach() {
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		SetStyle();
		
		auto context = GraphicsContext::G_Context[Render::ContextIndex()];

		
		GLFWwindow* window = static_cast<GLFWwindow*>(context->m_WindowHandle->GetNativeWindow());
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void OpenGLImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		auto context = GraphicsContext::G_Context[Render::ContextIndex()];
		io.DisplaySize = ImVec2((float)context->m_WindowHandle->GetWidth(), (float)context->m_WindowHandle->GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
	}

	ImTextureID OpenGLImGuiLayer::GetTextureID(std::shared_ptr<Texture> Texture)
	{
		return (ImTextureID)(intptr_t)Texture->GetTextureID();
	}

}