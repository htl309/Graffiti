#pragma once
#include"Event.h"

#include<sstream>

namespace Graffiti {

	class GRAFFITI_API KeyEvent :public Event {

	public:
		inline int GetKeyCode()const {return m_KeyCode;}

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);


	protected:
		KeyEvent(int  keycode): m_KeyCode(keycode) {} 

		//�����ʹ������ĸ����� "ASDW"
		int m_KeyCode;
	};

	class GRAFFITI_API KeyPressedEvent : public KeyEvent {
		public:
			KeyPressedEvent(int keycode, int repeatcount) :KeyEvent(keycode), m_RepeatCount(repeatcount) {};

			inline int GetRepeatCount() const {return m_RepeatCount;}

			std::string ToString() const override {
				std::stringstream ss;
				ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_RepeatCount << ")";
				return ss.str();
			}
			EVENT_CLASS_TYPE(KeyPressed);

		private:
			int m_RepeatCount;

	};

	class GRAFFITI_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class GRAFFITI_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
