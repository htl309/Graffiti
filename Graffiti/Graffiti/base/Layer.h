#pragma once
#include"gfpch.h"
#include"Core.h"
#include"Graffiti/Events/Event.h"
#include"TimeStep.h"
namespace Graffiti {
	class  Layer{

	public:

		Layer(const std::string& name = "layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep& timestep) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {};

		inline const std::string& GetName()const { return m_Name; }
	private:
		std::string m_Name;

	};

}
