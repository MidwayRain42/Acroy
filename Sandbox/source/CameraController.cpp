#include <Core/Input.hpp>
#include <Scene/Components.hpp>
#include "CameraController.hpp"

void CameraController::OnStart()
{
    // Acroy::Input::SetMouseMode(Acroy::MouseMode::Disabled);
    m_lastMousePos = Acroy::Input::GetMousePosition();
}

void CameraController::OnUpdate(Acroy::Timestep timestep)
{
    
    if (!m_active)
    {
        if (Acroy::Input::IsMouseButtonPressed(1))
        {
            Acroy::Input::SetMouseMode(Acroy::MouseMode::Disabled);
            m_lastMousePos = Acroy::Input::GetMousePosition();
            m_active = true;
        }
    }

    if (m_active)
    {
        if (!Acroy::Input::IsMouseButtonPressed(1))
        {
            Acroy::Input::SetMouseMode(Acroy::MouseMode::Normal);
            m_active = false;
        }
    }

    if (!m_active) return;

    float deltaTime = timestep.GetSeconds();
    auto& tc = GetComponent<Acroy::TransformComponent>();

    // --- Mouse look ---
    glm::vec2 mousePos = Acroy::Input::GetMousePosition();
    glm::vec2 delta = mousePos - m_lastMousePos;
    m_lastMousePos = mousePos;

    m_yaw   -= delta.x * mouseSensitivity;
    m_pitch -= delta.y * mouseSensitivity;

    // Clamp pitch to prevent gimbal flip
    m_pitch = glm::clamp(m_pitch, glm::radians(-89.0f), glm::radians(89.0f));

    tc.rotation.x = m_pitch;
    tc.rotation.y = m_yaw;

    // Extract axes directly from the transform matrix instead of recomputing
    // from angles — this stays consistent with however GetTransform() builds
    // the matrix (ZYX rotation order).
    glm::mat4 transform = tc.GetTransform();

    // Column 0 = local right, column 1 = local up, column 2 = local forward
    glm::vec3 right   =  glm::normalize(glm::vec3(transform[0]));
    glm::vec3 up      =  glm::normalize(glm::vec3(transform[1]));
    // View matrix is inverse(transform), so the camera looks down -Z in local space.
    glm::vec3 forward = -glm::normalize(glm::vec3(transform[2]));

    // --- Gather input ---
    glm::vec3 direction(0.0f);

    if (Acroy::Input::IsKeyPressed(87)) direction += forward; // W
    if (Acroy::Input::IsKeyPressed(83)) direction -= forward; // S
    if (Acroy::Input::IsKeyPressed(68)) direction += right;   // D
    if (Acroy::Input::IsKeyPressed(65)) direction -= right;   // A
    if (Acroy::Input::IsKeyPressed(69)) direction += up;      // E
    if (Acroy::Input::IsKeyPressed(81)) direction -= up;      // Q

    if (glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);

    float speed = moveSpeed;
    if (Acroy::Input::IsKeyPressed(340)) // Left Shift — sprint
        speed *= sprintMultiplier;
    
    if (Acroy::Input::IsKeyPressed(341)) // Left Ctrl — walk
        speed *= walkMultiplier;

    // --- Smooth velocity ---
    glm::vec3 targetVelocity = direction * speed;
    m_velocity = glm::mix(m_velocity, targetVelocity, glm::clamp(movementSmoothing * deltaTime, 0.0f, 1.0f));

    tc.position += m_velocity * deltaTime;
}