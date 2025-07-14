#pragma once
#include"Core.h"

namespace Graffiti {

	class  Input {

	public:
		Input() {};
		~Input() {};

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }

		inline static std::pair<float,float> GetMousePostion() { return s_Instance->GetMousePostionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		

	protected:
		virtual bool IsKeyPressedImpl(int keycode)  = 0;
		virtual bool IsMouseButtonPressedImpl(int button)  = 0;
		virtual std::pair<float, float> GetMousePostionImpl()  = 0;
		virtual float GetMouseXImpl()  = 0;
		virtual float GetMouseYImpl()  = 0;
	private:
		static Input* s_Instance;

	};

}