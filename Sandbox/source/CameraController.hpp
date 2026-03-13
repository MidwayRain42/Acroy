#pragma once

#include <Scene/ScriptableEntity.hpp>

class CameraController : public Acroy::ScriptableEntity
{

    float moveSpeed = 5.0f;
    float sprintMultiplier = 2.0f;
    float walkMultiplier = 0.3f;
    float movementSmoothing = 10.0f;
    float mouseSensitivity = 0.002f;
    
    void OnStart() override;
    void OnUpdate(Acroy::Timestep ts) override;

private:
    float m_yaw   = glm::radians(270.0f);
    float m_pitch = 0.0f;

    glm::vec2 m_lastMousePos { 0.0f };
    glm::vec3 m_velocity     { 0.0f };
    bool m_active = false;
};