#include"gfpch.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "OrthoCamera.h"


namespace Graffiti {
	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Graffiti::OrthoCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

}