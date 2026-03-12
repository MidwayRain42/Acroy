#include "CameraController.hpp"
#include <Scene/Components.hpp>
#include <Core/Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

static constexpr float YAW_OFFSET = 270.0f;

CameraController::CameraController(Acroy::Entity cameraEntity, float moveSpeed, float sensitivity)
: m_cameraEntity(cameraEntity)
, m_moveSpeed(moveSpeed)
, m_mouseSensitivity(sensitivity)
{}

// Read pitch/yaw straight from the transform
glm::vec3 CameraController::GetFront()
{
    // auto& tc = m_cameraEntity.GetComponent<Acroy::TransformComponent>();
    auto& tc = m_cameraEntity.GetComponent<Acroy::TransformComponent>();
    float pitch = glm::degrees(tc.rotation.x);
    float yaw   = glm::degrees(tc.rotation.y) + YAW_OFFSET;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}

glm::vec3 CameraController::GetRight()
{
    return glm::normalize(glm::cross(GetFront(), m_worldUp));
}

glm::vec3 CameraController::GetUp()
{
    return glm::normalize(glm::cross(GetRight(), GetFront()));
}

glm::mat4 CameraController::GetViewMatrix()
{
    auto& tc = m_cameraEntity.GetComponent<Acroy::TransformComponent>();
    return glm::lookAt(tc.position, tc.position + GetFront(), GetUp());
}

void CameraController::Activate()
{
    m_active     = true;
    m_firstMouse = true;
    Acroy::Input::SetMouseMode(Acroy::MouseMode::Disabled);
}

void CameraController::Deactivate()
{
    m_active   = false;
    m_velocity = glm::vec3(0.0f);
    Acroy::Input::SetMouseMode(Acroy::MouseMode::Normal);
}

void CameraController::OnUpdate(Acroy::Timestep ts)
{
    if (!m_active) return;

    UpdateMouse();
    UpdateMovement(ts.GetSeconds());
}

void CameraController::UpdateMouse()
{
    glm::vec2 mousePos = Acroy::Input::GetMousePosition();

    if (m_firstMouse)
    {
        m_lastMousePos = mousePos;
        m_firstMouse   = false;
        return;
    }

    glm::vec2 delta = mousePos - m_lastMousePos;
    m_lastMousePos  = mousePos;

    auto& tc = m_cameraEntity.GetComponent<Acroy::TransformComponent>();

    tc.rotation.y -= glm::radians(delta.x * m_mouseSensitivity);
    tc.rotation.x -= glm::radians(delta.y * m_mouseSensitivity);
    tc.rotation.x  = glm::clamp(tc.rotation.x, glm::radians(-89.0f), glm::radians(89.0f));
}

void CameraController::UpdateMovement(float dt)
{
    auto& tc = m_cameraEntity.GetComponent<Acroy::TransformComponent>();

    glm::vec3 front = GetFront();
    glm::vec3 right = GetRight();
    glm::vec3 up    = GetUp();

    glm::vec3 direction(0.0f);

    float speed = m_moveSpeed;
    if (Acroy::Input::IsKeyPressed(341)) speed *= 0.3f;
    if (Acroy::Input::IsKeyPressed(340)) speed *= 2.0f;

    if (Acroy::Input::IsKeyPressed(87)) direction += front;
    if (Acroy::Input::IsKeyPressed(83)) direction -= front;
    if (Acroy::Input::IsKeyPressed(68)) direction += right;
    if (Acroy::Input::IsKeyPressed(65)) direction -= right;
    if (Acroy::Input::IsKeyPressed(69)) direction += up;
    if (Acroy::Input::IsKeyPressed(81)) direction -= up;

    glm::vec3 targetVelocity(0.0f);
    if (glm::length(direction) > 0.0f)
        targetVelocity = glm::normalize(direction) * speed;

    m_velocity = glm::mix(m_velocity, targetVelocity, 1.0f - std::exp(-dt / m_smoothTime));

    tc.position += m_velocity * dt;
}