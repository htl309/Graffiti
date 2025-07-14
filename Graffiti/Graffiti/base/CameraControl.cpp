#include"gfpch.h"
#include "CameraControl.h"
#include "Input.h"
#include "Graffiti/base/KMCodes.h"

namespace Graffiti {
    CameraControl::CameraControl(glm::vec3 position, glm::vec3 target, float aspect)
        :m_aspect(aspect)
    {
        m_camera = std::make_shared<Camera>(position, target, glm::vec3{ 0,1,0 }, 45.0, m_aspect, 0.1, 1000);

    }
    CameraControl::~CameraControl()
    {
    }
    void CameraControl::KeyMovement(TimeStep& ts)
    {
        if (Input::IsKeyPressed(GF_KEY_A))
            m_camera->ProcessKeyboard(CameraMoveDir::LEFT, ts);
        if (Input::IsKeyPressed(GF_KEY_S))
            m_camera->ProcessKeyboard(CameraMoveDir::DOWN, ts);
        if (Input::IsKeyPressed(GF_KEY_D))
            m_camera->ProcessKeyboard(CameraMoveDir::RIGHT, ts);
        if (Input::IsKeyPressed(GF_KEY_W))
            m_camera->ProcessKeyboard(CameraMoveDir::UP, ts);
        if (Input::IsKeyPressed(GF_KEY_UP))
            m_camera->ProcessKeyboard(CameraMoveDir::FOWARD, ts);
        if (Input::IsKeyPressed(GF_KEY_DOWN))
            m_camera->ProcessKeyboard(CameraMoveDir::BACKWARD, ts);
    }
    void CameraControl::MouseMovement(TimeStep& ts)
    {
        if (Input::IsMouseButtonPressed(GF_MOUSE_BUTTON_LEFT)) {
            if (m_firstMouse == true) {
                lastX = Input::GetMouseX();
                lastY = Input::GetMouseY();
                m_firstMouse = false;
            }

            float dx = Input::GetMouseX() - lastX;
            float dy = Input::GetMouseY() - lastY;

            m_camera->ProcessMouseMovement(dx, dy, ts);
            lastX = Input::GetMouseX();
            lastY = Input::GetMouseY();

        }
        else {
            m_firstMouse = true;
        }
    }

    void CameraControl::MouseScroll(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(CameraControl::OnWindowResize));
    }

    void CameraControl::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(CameraControl::OnWindowResize));
        dispatcher.Dispatch< MouseScrolledEvent >(GF_BIND_EVENT_FN(CameraControl::OnFovReset));
    }

    bool  CameraControl::OnWindowResize(WindowResizeEvent& e) {
        m_aspect = float(e.GetWidth()) / float(e.GetHeight());
        m_camera->UpdateProjectionMatrix(m_fov, m_aspect, 0.1f, 1000.0f);
        return false;
    }
    bool CameraControl::OnFovReset(MouseScrolledEvent& e)
    {
        m_fov -= e.GetYOffset();
        if (m_fov < 1) m_fov = 1;
        else if (m_fov > 89) m_fov = 89;
        m_camera->UpdateProjectionMatrix(m_fov, m_aspect, 0.1f, 1000.0f);
        return false;
    }
}