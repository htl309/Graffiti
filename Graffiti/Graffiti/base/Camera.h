#pragma once
#include"glm//glm.hpp"
#include"Graffiti/Scene/Model.h"
namespace Graffiti {

	enum class CameraMoveDir
	{
		FOWARD,
		BACKWARD,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:

		Camera(glm::vec3 _position=glm::vec3(6,6,6), glm::vec3 _target = glm::vec3(0, 0, 0), glm::vec3 _upVector=glm::vec3(0,1,0), float fov = 45.0, float aspect = 15.0 / 13.0, float znear = 0.1, float zfar = 1000.0);
		~Camera() {};
		void UpdateProjectionMatrix(float fov, float aspect, float znear, float zfar);

		void ResetViewMatrix(glm::vec3 _position, glm::vec3 _target, glm::vec3 _upVector);

        //根据包围盒计算观察目标
        void SetTarget(BoundingBox& b);
		// 交互控制
		void ProcessKeyboard(CameraMoveDir direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime);

		//// 可选：缩放（鼠标滚轮）
		//void ProcessMouseScroll(float yoffset);
		
		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		glm::vec3 GetCameraPosition() const { return m_Position; }
	
	private:
        void UpdateViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Target;
		glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Position = { 0,0,0 };

        float m_Distance;         // 相机到目标的距离
        float m_Azimuth;          // 方位角（水平旋转）
        float m_Elevation;        // 仰角（垂直旋转）
        float m_Sensitivity=20.0;      // 鼠标灵敏度


		float m_MovementSpeed = 3.0f;

	};

}