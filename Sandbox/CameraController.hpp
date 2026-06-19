#pragma once

#include <PerspectiveCamera.hpp>
#include <Transform.hpp>
#include <Input.hpp>
#include <Events.hpp>

#include <glm/glm.hpp>

class CameraController
{
public:
    CameraController() = default;

    CameraController(Acroy::PerspectiveCamera& camera)
    {
        SetCamera(camera);
    }

    void SetCamera(Acroy::PerspectiveCamera& camera)
    {
        m_camera = &camera;
        Reset();
    }

    void SetSensitivity(float sens) { m_sensitivity = sens; }
    void SetMoveSpeed(float speed)  { m_moveSpeed = speed; }

    void OnEvent(Acroy::Event& event)
    {
        if (event.GetEventType() == Acroy::EventType::MouseMoved)
        {
            auto& e = static_cast<Acroy::MouseMovedEvent&>(event);

            if (m_firstMouse)
            {
                m_lastMouse = { e.GetX(), e.GetY() };
                m_firstMouse = false;
            }

            glm::vec2 delta = glm::vec2{ e.GetX(), e.GetY() } - m_lastMouse;
            m_lastMouse = { e.GetX(), e.GetY() };

            m_targetYaw   -= delta.x * m_sensitivity;
            m_targetPitch -= delta.y * m_sensitivity;

            const float limit = glm::radians(89.0f);
            m_targetPitch = glm::clamp(m_targetPitch, -limit, limit);
        }
    }

    void OnUpdate(float dt)
    {
        // float lookT = 1.0f - std::exp(-18.0f * dt);
        // m_currentYaw   = glm::mix(m_currentYaw, m_targetYaw, lookT);
        // m_currentPitch = glm::mix(m_currentPitch, m_targetPitch, lookT);

        Acroy::Transform& t = m_camera->GetTransform();
        // t.SetRotation({ m_currentPitch, m_currentYaw, 0.0f });
        t.SetRotation({ m_targetPitch, m_targetYaw, 0.0f });

        glm::vec3 forward = t.Forward();
        glm::vec3 right   = t.Right();
        glm::vec3 up      = t.Up();

        float speed = m_moveSpeed;
        if (Acroy::Input::IsKeyPressed(340)) speed *= 2.0f; // Left Shift
        if (Acroy::Input::IsKeyPressed(341)) speed *= 0.4f; // Left Ctrl

        glm::vec3 input(0.0f);
        if (Acroy::Input::IsKeyPressed(87)) input += forward; // W
        if (Acroy::Input::IsKeyPressed(83)) input -= forward; // S
        if (Acroy::Input::IsKeyPressed(65)) input -= right;   // A
        if (Acroy::Input::IsKeyPressed(68)) input += right;   // D
        if (Acroy::Input::IsKeyPressed(69)) input += up;      // E
        if (Acroy::Input::IsKeyPressed(81)) input -= up;      // Q

        glm::vec3 targetVel(0.0f);
        if (glm::length(input) > 0.0001f)
            targetVel = glm::normalize(input) * speed;

        float moveT = 1.0f - std::exp(-12.0f * dt);
        m_velocity = glm::mix(m_velocity, targetVel, moveT);

        t.Translate(m_velocity * dt);
    }

    void Reset()
    {
        m_targetYaw   = -glm::half_pi<float>();
        m_targetPitch = 0.0f;

        m_currentYaw   = m_targetYaw;
        m_currentPitch = m_targetPitch;

        m_velocity = glm::vec3{0.0f};

        Acroy::Input::SetMouseMode(Acroy::MouseMode::Disabled);
    }

private:
    Acroy::PerspectiveCamera* m_camera;

    float m_yaw   = -glm::half_pi<float>();
    float m_pitch = 0.0f;

    float m_targetYaw = 0.0f;
    float m_targetPitch = 0.0f;

    float m_currentYaw = 0.0f;
    float m_currentPitch = 0.0f;

    float m_sensitivity = 0.0025f;
    float m_moveSpeed   = 5.0f;

    glm::vec2 m_lastMouse {0.0f};
    bool m_firstMouse = true;

    glm::vec3 m_velocity {0.0f};
};