#pragma once

#include "Types.hpp"

namespace Acroy
{
    class Event;
    class Renderer;

    class Layer {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(f32 deltaTime) {}
        virtual void OnRender(Renderer& renderer) {}
    };
}