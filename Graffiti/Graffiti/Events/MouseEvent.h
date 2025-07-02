#pragma once

#include"Event.h"

#include<sstream>
namespace Graffiti {
	class  MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << " , " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	private:
		float m_MouseX, m_MouseY;
	};

	class  MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float x, float y) :m_OffsetX(x), m_OffsetY(y) {}
		inline float GetXOffset() { return m_OffsetX; }
		inline float GetYOffset() { return m_OffsetY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << " , " << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	private:
		float m_OffsetX, m_OffsetY;
	};

	class  MouseButtonEvent : public Event {
	public:

		inline int GetMouseButton() { return m_Buttom; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton);
	protected:
		MouseButtonEvent(int Button) :m_Buttom(Button) {}
		int m_Buttom;
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int Button) :MouseButtonEvent(Button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Buttom;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed);
	};
	class  MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int Button) :MouseButtonEvent(Button) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Buttom;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}