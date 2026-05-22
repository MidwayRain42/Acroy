#pragma once

#include "Types.hpp"

namespace Acroy
{
    class Window;
    class FrameBuffer;

    class Mesh;

    struct RendererDesc
    {
        Window* window;
        bool enableVSync;
    };

    class Renderer
    {
    public:
        Renderer(const RendererDesc& desc);
        ~Renderer();

        void BeginFrame();
        void EndFrame();

        void Update(f32 deltaTime);

    private:
        Window* _window;
        FrameBuffer* _frameBuffer;
    };
}