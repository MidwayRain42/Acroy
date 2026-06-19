#pragma once

#include "Types.hpp"
#include "Transform.hpp"

namespace Acroy
{
    // struct PointLight
    // {
    //     f32 radius = 1.0f;
    //     f32 strength = 1.0f;
    //     glm::vec3 color = { 1.0f, 1.0f, 1.0f };
    //     glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    // };


    struct alignas(16) PointLight {
        float radius = 1.0f;
        float strength = 1.0f;
        alignas(16) glm::vec3 color = glm::vec3(1.0f);
        alignas(16) glm::vec3 pos = glm::vec3(0.0f);
    };
}