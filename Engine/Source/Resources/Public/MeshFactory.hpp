#pragma once

#include "Types.hpp"

namespace Acroy
{
    class Mesh;
    class MeshFactory
    {
    public:
        static Mesh* CreateQuad(f32 width = 1.0f, f32 height = 1.0f);
    };
}