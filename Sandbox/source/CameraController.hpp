#pragma once

#include <Scene/Entity.hpp>
#include <glm/glm.hpp>

class CameraController
{
public:
    CameraController(Acroy::Entity cameraEntity, float moveSpeed = 3.0f, float sensitivity = 0.1f);

    void OnUpdate(Acroy::Timestep ts);
    glm::mat4 GetViewMatrix();

    void Activate();
    void Deactivate();
    bool IsActive() const { return m_active; }

    void SetSpeed(float speed) { m_moveSpeed = speed; }
    void SetSensitivity(float s) { m_mouseSensitivity = s; }
    void SetSmoothTime(float t) { m_smoothTime = t; }

private:
    void UpdateMouse();
    void UpdateMovement(float dt);
    glm::vec3 GetFront();
    glm::vec3 GetRight();
    glm::vec3 GetUp();

private:
    Acroy::Entity m_cameraEntity;

    bool m_active = false;

    float m_moveSpeed        = 3.0f;
    float m_mouseSensitivity = 0.1f;
    float m_smoothTime       = 0.1f;

    bool      m_firstMouse = true;
    glm::vec2 m_lastMousePos { 0.0f };
    glm::vec3 m_velocity { 0.0f };

    const glm::vec3 m_worldUp { 0.0f, 1.0f, 0.0f };
};