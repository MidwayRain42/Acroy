#pragma once

#include "Types.hpp"
#include <glm/vec3.hpp>

namespace Acroy
{
    class Mesh;
    class MeshFactory
    {
    public:
        static Mesh* CreateQuad(f32 width = 1.0f, f32 height = 1.0f);
        static Mesh* CreateCube(const glm::vec3& size = glm::vec3(1.0f));
    };
}