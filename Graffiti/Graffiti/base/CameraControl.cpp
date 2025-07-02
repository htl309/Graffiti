#include"gfpch.h"
#include "CameraControl.h"
#include "Input.h"
#include "Graffiti/base/KMCodes.h"

namespace Graffiti {
    CameraControl::CameraControl(glm::vec3 position, glm::vec3 target, float aspect)
    {
        m_camera = std::make_shared<Camera>(position, target, glm::vec3{ 0,1,0 }, 45.0, aspect, 0.1, 1000);

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

    void CameraControl::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch< WindowResizeEvent >(GF_BIND_EVENT_FN(CameraControl::OnWindowResize));
    }

    bool  CameraControl::OnWindowResize(WindowResizeEvent& e) {

        m_camera->UpdateProjectionMatrix(45.0, float(e.GetWidth()) / float(e.GetHeight()), 0.1f, 1000.0f);
        return false;
    }
}