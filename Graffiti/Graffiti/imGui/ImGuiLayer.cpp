#include"gfpch.h"

#include <GLFW/glfw3.h>
#include"imGuiLayer.h"

#include<imgui.h>

#include <examples/imgui_impl_glfw.h>
#include<examples/imgui_impl_opengl3.h>


#include "Graffiti/Application.h"

namespace Graffiti {
	
		ImGuiLayer::ImGuiLayer():Layer("ImGuiLayer"){ GF_ASSERT("imgui!!!!!!!") }


		ImGuiLayer::~ImGuiLayer() {}

		
		void ImGuiLayer::OnAttach()  {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}
		



		void ImGuiLayer::OnDetach() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		


		

		void ImGuiLayer::Begin()
		{
			
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
		}

		void ImGuiLayer::end()
		{
			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::Get();
			io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

		}

		void ImGuiLayer::OnImGuiRender()
		{
			static bool show = 1;

			ImGui::Begin("Hello");
			ImGui::Checkbox("e", &show);
			ImGui::End();
			if(show)
			ImGui::ShowDemoWindow(&show);
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


