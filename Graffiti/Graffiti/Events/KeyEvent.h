#pragma once
#include"Event.h"

#include<sstream>

namespace Graffiti {
	class  KeyEvent :public Event {
	public:
		inline int GetKeyCode()const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		KeyEvent(int  keycode) : m_KeyCode(keycode) {}

		//用整型代表是哪个按键 "ASDW"
		int m_KeyCode;
	};

	class  KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatcount) :KeyEvent(keycode), m_RepeatCount(repeatcount) {};

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_RepeatCount << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int m_RepeatCount;
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
