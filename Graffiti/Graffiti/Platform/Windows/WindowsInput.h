#pragma once
#include"Graffiti/Input.h"

namespace Graffiti {

	class GRAFFITI_API WindowsInput:public Input{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;

		virtual std::pair<float, float> GetMousePostionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}

