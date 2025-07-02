#pragma once
#include "gfpch.h"
#include "Graffiti/base/Core.h"

#include<string>
#include<functional>

namespace Graffiti {
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	//同一个事件可以属于多个类别，
	//比如MouseButtonPressed，属于输入事件也属于鼠标事件等，其类别是11010
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = GF_BIT(0),	//0000 0000
		EventCategoryInput = GF_BIT(1),			//0000 0001
		EventCategoryKeyboard = GF_BIT(2),		//0000 0010
		EventCategoryMouse = GF_BIT(3),			//0000 0100
		EventCategoryMouseButton = GF_BIT(4)	//0000 1000
	};

	//两个宏定义，这样子在子类中就不用重复写函数的定义了
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::type; } \
									virtual EventType GetEventType()const override{return GetStaticType();} \
									virtual const char* GetName() const override { return #type; };

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override{ return category;}

	class  Event {
		//friend class;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCateGory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

		//代表事件是否被处理
		bool m_Handled = false;
	};
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}