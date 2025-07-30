#pragma once
#include"gfpch.h"
#include"Graffiti/base/core.h"
#include"glm/glm.hpp"
#include "Camera.h"
#include"Graffiti/base/TimeStep.h"
#include "Graffiti/Events/ApplicationEvent.h"
#include "Graffiti/Events/MouseEvent.h"
namespace Graffiti {
    class CameraControl {
    public:
        CameraControl(glm::vec3 position = glm::vec3{ 6,6,6 }, glm::vec3 target = glm::vec3{ 0,0,0 },float aspect=15.0/13.0);
        ~CameraControl();

        void KeyMovement(TimeStep& ts);
        void MouseMovement();
        void MouseScroll(Event& event);

        void OnEvent(Event& event);

        glm::mat4& GetViewProjectionMatrix() const { return m_camera->GetViewProjectionMatrix(); }
        glm::vec3& GetCameraPosition() const { return m_camera->GetCameraPosition(); }

        //根据包围盒计算观察目标
        void SetTarget(BoundingBox& b) { m_camera->SetTarget(b); }
    private:
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnFovReset(MouseScrolledEvent& e);

        std::shared_ptr <Camera> m_camera;
        float lastX = 0.0;
        float lastY = 0.0;
        bool m_firstMouse = true;
        float m_fov = 45.0;
        float m_aspect;

    };

}