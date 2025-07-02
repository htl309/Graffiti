#pragma once
#include "gfpch.h"
#include "Core.h"

#include"Graffiti/Events/Event.h"
namespace Graffiti {
	
	class 
        TimeStep {
	public:
		TimeStep(float time = 0.0f):m_Time(time) {

		}
		operator float() { return m_Time; }
		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}