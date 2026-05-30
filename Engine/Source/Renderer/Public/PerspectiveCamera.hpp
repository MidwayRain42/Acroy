#pragma once

#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Acroy
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera() = default;
        PerspectiveCamera(float fov, float aspect, float zNear, float zFar)
        : m_fov(fov)
        , m_aspect(aspect)
        , m_zNear(zNear)
        , m_zFar(zFar)
        {
            RecalculateProjection();
        }

        void SetPerspective(float fov, float aspect, float zNear, float zFar) {
            m_fov    = fov;
            m_aspect = aspect;
            m_zNear  = zNear;
            m_zFar   = zFar;
            RecalculateProjection();
        }

        void SetFOV(float fov) {
            m_fov = fov;
            RecalculateProjection();
        }

        void SetZNear(float zNear) {
            m_zNear = zNear;
            RecalculateProjection();
        }

        void SetZFar(float zFar) {
            m_zFar = zFar;
            RecalculateProjection();
        }

        void Resize(float width, float height) override
        {
            m_aspect = width / height;
            RecalculateProjection();
        }

        float GetFOV()    const { return m_fov;    }
        float GetAspect() const { return m_aspect; }
        float GetZNear()  const { return m_zNear;  }
        float GetZFar()   const { return m_zFar;   }

        void SetProjection(const glm::mat4& proj) { m_proj = proj; }

    private:
        float m_aspect = 1.0f;
        float m_fov    = 60.0f;
        float m_zNear  = 0.1f;
        float m_zFar   = 1000.0f;

    private:
        void RecalculateProjection()
        {
            m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_zNear, m_zFar);
        }
    };
}