#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

namespace Acroy
{
    struct Transform
    {
        glm::vec3 position {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation {0.0f, 0.0f, 0.0f}; // Euler radians (pitch, yaw, roll)
        glm::vec3 scale    {1.0f, 1.0f, 1.0f};

        Transform() = default;
        Transform(const glm::vec3& pos) : position(pos) {}

        // ---------------------------------------------------------------------
        // Position helpers
        // ---------------------------------------------------------------------

        void Translate(const glm::vec3& delta)
        {
            position += delta;
        }

        void SetPosition(const glm::vec3& pos)
        {
            position = pos;
        }

        // ---------------------------------------------------------------------
        // Rotation helpers
        // ---------------------------------------------------------------------

        void RotateEuler(const glm::vec3& eulerDelta)
        {
            rotation += eulerDelta;
        }

        void RotateX(float radians)
        {
            rotation.x += radians;
        }

        void RotateY(float radians)
        {
            rotation.y += radians;
        }

        void RotateZ(float radians)
        {
            rotation.z += radians;
        }

        void SetRotation(const glm::vec3& eulerRadians)
        {
            rotation = eulerRadians;
        }

        // ---------------------------------------------------------------------
        // Scale helpers
        // ---------------------------------------------------------------------

        void ScaleBy(const glm::vec3& factor)
        {
            scale *= factor;
        }

        void ScaleUniform(float factor)
        {
            scale *= factor;
        }

        void SetScale(const glm::vec3& newScale)
        {
            scale = newScale;
        }

        // ---------------------------------------------------------------------
        // Matrix helpers
        // ---------------------------------------------------------------------

        glm::mat4 GetRotationMatrix() const
        {
            return
                glm::rotate(glm::mat4(1.0f), rotation.z, {0, 0, 1}) *
                glm::rotate(glm::mat4(1.0f), rotation.y, {0, 1, 0}) *
                glm::rotate(glm::mat4(1.0f), rotation.x, {1, 0, 0});
        }

        glm::mat4 GetTranslationMatrix() const
        {
            return glm::translate(glm::mat4(1.0f), position);
        }

        glm::mat4 GetScaleMatrix() const
        {
            return glm::scale(glm::mat4(1.0f), scale);
        }

        glm::mat4 GetMatrix() const
        {
            return
                GetTranslationMatrix() *
                GetRotationMatrix() *
                GetScaleMatrix();
        }

        // ---------------------------------------------------------------------
        // Direction vectors
        // ---------------------------------------------------------------------

        glm::vec3 Forward() const
        {
            return glm::normalize(
                glm::vec3(GetRotationMatrix() * glm::vec4(0, 0, -1, 0)));
        }

        glm::vec3 Right() const
        {
            return glm::normalize(
                glm::vec3(GetRotationMatrix() * glm::vec4(1, 0, 0, 0)));
        }

        glm::vec3 Up() const
        {
            return glm::normalize(
                glm::vec3(GetRotationMatrix() * glm::vec4(0, 1, 0, 0)));
        }
    };
}