#pragma once

#include "Layer.hpp"

#include <memory>
#include <vector>

namespace Acroy
{
    class Window;
    class Event;
    class Renderer;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void OnEvent(Event& event);
        void Run();

        template<typename TLayer, typename... Args>
        TLayer* PushLayer(Args&&... args)
        {
            static_assert(std::is_base_of_v<Layer, TLayer>);

            auto layer = std::make_unique<TLayer>(
                std::forward<Args>(args)...
            );

            TLayer* raw = layer.get();

            _layers.push_back(std::move(layer));
            raw->OnAttach();

            return raw;
        }

        void PopLayer(Layer* layer);

    private:
        std::vector<std::unique_ptr<Layer>> _layers;

        std::unique_ptr<Window>   _window;
        std::unique_ptr<Renderer> _renderer;

        bool _running = true;
    };
}