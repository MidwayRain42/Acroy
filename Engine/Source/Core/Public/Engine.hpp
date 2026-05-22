#pragma once

#include "Window.hpp"

namespace Acroy
{
    class FrameBuffer;
    class GraphicsPipeline;
    class Buffer;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Run();

    private:
        Window* _window;
        FrameBuffer* _frameBuffer;
        GraphicsPipeline* _pipeline;
        Buffer* _vertexBuffer;
        Buffer* _indexBuffer;
    };
}
