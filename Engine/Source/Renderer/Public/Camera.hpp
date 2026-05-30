#pragma once

#include "Transform.hpp"

namespace Acroy
{
    class Camera
    {
    public:
        Camera(const glm::mat4& proj) : m_proj(proj) {}

        Camera() = default;
        virtual ~Camera() = default;

        const glm::mat4& GetProj() const { return m_proj; }

        virtual void Resize(float width, float height) {}

        Transform& GetTransform() { return m_transform; }
    
    protected:
        glm::mat4 m_proj { 1.0f };
        Transform m_transform;
    };
}