#include"gfpch.h"
#include"Camera.h"
#include"Graffiti/Render/Renderer.h"

#include"Window.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


namespace Graffiti {
	Camera::Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _upVector, float fov, float aspect, float znear, float zfar)
		:m_Position(_position), m_Target(_target), m_Up(_upVector)
	{
        m_Right = glm::normalize(glm::cross(m_Target - m_Position, _upVector));
		m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);

        m_Distance = glm::length(m_Position - m_Target);
        glm::vec3 dir = (m_Position - m_Target) / m_Distance;
        m_Elevation = glm::degrees(asin(dir.y));
        m_Azimuth = glm::degrees(atan2(dir.x, dir.z));
           
		UpdateProjectionMatrix(fov, aspect, znear,zfar);
	}
	void Camera::UpdateProjectionMatrix(float fov, float aspect, float znear, float zfar)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, znear, zfar);
		if(Render::GetRenderAPI() == RenderAPI::API::Vulkan)
		m_ProjectionMatrix[1][1] *= -1;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::UpdateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void Camera::ResetViewMatrix(glm::vec3 _position, glm::vec3 _target, glm::vec3 _upVector)
	{
		m_Position = _position;
		m_Target = _target;
        m_Distance = glm::distance(_target, _position);
		m_Up = _upVector;
		m_Right = glm::normalize(glm::cross(m_Target - m_Position, _upVector));
		UpdateViewMatrix();
	}
  
    void Camera::SetTarget(BoundingBox& b)
    {
       m_Target = (b.maxPos + b.minPos) * 0.5f;
       float dist = glm::distance(b.maxPos, b.minPos);
       m_Position = m_Target + glm::vec3(-dist, 0, dist);
       
       m_Distance = glm::length(m_Position - m_Target);
       glm::vec3 dir = (m_Position - m_Target) / m_Distance;
       m_Elevation = glm::degrees(asin(dir.y));
       m_Azimuth = glm::degrees(atan2(dir.x, dir.z));


       ResetViewMatrix(m_Position,m_Target,m_Up);
    }

    void Camera::ProcessKeyboard(CameraMoveDir direction, float deltaTime)
	{
		
		switch (direction)
		{
		case Graffiti::CameraMoveDir::FOWARD:
		{
			m_Position += glm::normalize(m_Target - m_Position) * m_MovementSpeed * deltaTime;
            m_Target += glm::normalize(m_Target - m_Position) * m_MovementSpeed * deltaTime;
			break;
		}
		case Graffiti::CameraMoveDir::BACKWARD:
		{
			m_Position -= glm::normalize(m_Target - m_Position) * m_MovementSpeed * deltaTime;
            m_Target -= glm::normalize(m_Target - m_Position) * m_MovementSpeed * deltaTime;
			break;
		}
		case Graffiti::CameraMoveDir::UP:
		{
			m_Position += m_Up * m_MovementSpeed * deltaTime;
            m_Target += m_Up * m_MovementSpeed * deltaTime;
			break;
		}
		case Graffiti::CameraMoveDir::DOWN:
		{
			m_Position -= m_Up * m_MovementSpeed * deltaTime;
			m_Target -= m_Up * m_MovementSpeed * deltaTime;
			break;
		}
		case Graffiti::CameraMoveDir::LEFT:
		{
			m_Position -= m_Right * m_MovementSpeed * deltaTime;
			m_Target -= m_Right * m_MovementSpeed * deltaTime;
			break;
		}

		case Graffiti::CameraMoveDir::RIGHT:
		{
			m_Position += m_Right * m_MovementSpeed * deltaTime;
			m_Target += m_Right * m_MovementSpeed * deltaTime;
			break;
		}
		default:
			break;
		}
        UpdateViewMatrix();
	}
    void Camera::ProcessMouseMovement(float xoffset, float yoffset)
    {
        m_Azimuth -= xoffset * m_Sensitivity;
        m_Elevation += yoffset * m_Sensitivity;

        // 限制仰角范围（-89°到89°之间）
        if (m_Elevation > 89.0f) m_Elevation = 89.0f;
        if (m_Elevation < -89.0f) m_Elevation = -89.0f;

        // 确保方位角在0-360°范围内
        if (m_Azimuth < 0.0f) m_Azimuth += 360.0f;
        if (m_Azimuth > 360.0f) m_Azimuth -= 360.0f;

        // 球坐标转笛卡尔坐标
        float x = m_Target.x + m_Distance * cos(glm::radians(m_Elevation)) * sin(glm::radians(m_Azimuth));
        float y = m_Target.y + m_Distance * sin(glm::radians(m_Elevation));
        float z = m_Target.z + m_Distance * cos(glm::radians(m_Elevation)) * cos(glm::radians(m_Azimuth));
  
        ResetViewMatrix({ x,y,z }, m_Target, { 0,1,0 });
    }
}