#include"gfpch.h"

#include <GLFW/glfw3.h>
#include"imGuiLayer.h"

#include "Graffiti/ImGui/imgui.h"

#include "Graffiti/Render/Renderer.h"
#include "Graffiti/Render/GraphicsContext.h"

#include "Graffiti/Graphic/OpenGL/OpenGLImGuiLayer.h"
#include "Graffiti/Graphic/Vulkan/VulkanImGuiLayer.h"
namespace Graffiti {
	
	int ImGuiLayer::frameCount = 0;
	float ImGuiLayer::fps = 0.0f;
	float ImGuiLayer::lastTime = 0.0f;

	ImGuiLayer::ImGuiLayer() :Layer("ImGuiLayer") {
	}

	ImGuiLayer::~ImGuiLayer() {}


	void ImGuiLayer::OnImGuiRender()
	{
		//计算帧率
		{
			float currentTime = static_cast<float>(ImGui::GetTime());  // 或使用你自己的高精度计时函数

			frameCount++;

			// 每隔1秒更新一次 FPS
			if (currentTime - lastTime >= 1.0f) {
				fps = frameCount / (currentTime - lastTime);
				frameCount = 0;
				lastTime = currentTime;
			}
		}


			ImGui::Begin(GraphicsContext::GraphicAPI_Version.data());
			ImGui::SetWindowPos(ImVec2(40, 30), ImGuiCond_Always);
			ImGui::SetWindowSize(ImVec2(420, 155), ImGuiCond_Always);

			ImGui::Text(("GPU:" + GraphicsContext::PhysicalGPU_Name).data());
			ImGui::Text("Window Size: %.0f x %.0f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
			ImGui::Text("VSync/Real FPS: %.1f/%0.2f", ImGui::GetIO().Framerate,fps);
	

			ImGui::End();
		
	}


	void ImGuiLayer::SetStyle()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.FontGlobalScale = 2.0f;
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
	}
	
	ImGuiLayer* ImGuiLayer::CreateImGuiLayer()
	{

		if (Render::GetRenderAPI() == RenderAPI::API::OpenGL) {
			return new OpenGLImGuiLayer();
		}
		else if (Render::GetRenderAPI() == RenderAPI::API::Vulkan) {
			return new VulkanImGuiLayer();
		}
		return nullptr;

	}
}

/*void ImGuiLayer::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<KeyPressedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<KeyTypedEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	dispatcher.Dispatch<WindowResizeEvent>(GF_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.GetMouseButton()] = true;

	return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[e.GetMouseButton()] = false;

	return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(e.GetX(),e.GetY());

	return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel  += e.GetXOffset();
	io.MouseWheelH += e.GetYOffset();

	return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysData[e.GetKeyCode()].Down = true;

	io.KeyCtrl = io.KeysData[GLFW_KEY_LEFT_CONTROL].Down || io.KeysData[GLFW_KEY_RIGHT_CONTROL].Down;
	io.KeyShift = io.KeysData[GLFW_KEY_LEFT_SHIFT].Down || io.KeysData[GLFW_KEY_RIGHT_SHIFT].Down;
	io.KeyAlt = io.KeysData[GLFW_KEY_LEFT_ALT].Down || io.KeysData[GLFW_KEY_RIGHT_ALT].Down;
	io.KeySuper = io.KeysData[GLFW_KEY_LEFT_SUPER].Down || io.KeysData[GLFW_KEY_RIGHT_SUPER].Down;

	return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysData[e.GetKeyCode()].Down = false;
	return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	int keycode = e.GetKeyCode();
	if (keycode > 0 && keycode < 0x10000) {
		io.AddInputCharacter((unsigned short)keycode);
	}
	return false;
}

bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	return false;
}*/