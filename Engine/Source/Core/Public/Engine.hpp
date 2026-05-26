#pragma once

#include "Window.hpp"
#include <memory>

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
        std::unique_ptr<Window>         _window;
        std::unique_ptr<Renderer>       _renderer;

        std::shared_ptr<Mesh>           _mesh;
        std::shared_ptr<SimpleMaterial> _mat;

    };
}
