#include "gfpch.h"
#include "WindowsInput.h"
#include "Graffiti/base/Application.h"
#include<GLFW/glfw3.h>

namespace Graffiti {
	Input* Input::s_Instance = new WindowsInput();
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow());
		auto state = glfwGetKey(window, keycode);
		return state ==GLFW_PRESS|| state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS ; 

	}

	std::pair<float, float> WindowsInput::GetMousePostionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		
		return { (float)xpos,(float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		return GetMousePostionImpl().first;
	}

	float WindowsInput::GetMouseYImpl()
	{
		return GetMousePostionImpl().second;
	}

}

