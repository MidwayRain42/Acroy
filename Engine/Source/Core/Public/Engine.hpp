#pragma once

#include "Window.hpp"

namespace Acroy
{
    class Renderer;
    class Mesh;
    class SimpleMaterial;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

    private:
        Window* _window;

        Mesh* _mesh;
        SimpleMaterial* _mat;

        Renderer* _renderer;
    };
}
