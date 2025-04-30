#pragma once
#include"gfpch.h"
#include"Core.h"
#include"Events/Event.h"
namespace Graffiti {
	class GRAFFITI_API Layer{

	public:

		Layer(const std::string& name = "layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {};

		inline const std::string& GetName()const { return m_DebugName; }
	private:
		std::string m_DebugName;

	};

}
