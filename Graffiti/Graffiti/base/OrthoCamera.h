#pragma once
#include"glm/glm.hpp"

namespace Graffiti {

	class OrthoCamera {
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		glm::mat4& GetViewProjectionMatrix() {return m_ViewProjectionMatrix;}

		glm::vec3& GetPostion() { return m_Position; }
		void SetPositon(const glm::vec3& postion) {m_Position = postion; RecalculateViewMatrix(); }

		float& GetRotation() { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
	private:

		void RecalculateViewMatrix();
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;


		glm::vec3 m_Position = { 0,0,0 };
		

		float m_Rotation =0.0f;
	};
}