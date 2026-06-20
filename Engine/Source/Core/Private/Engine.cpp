#include "Types.hpp"
#include "Engine.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

#include <iostream>
#include <ranges>
#include <algorithm>

namespace Acroy
{
    Engine::Engine() : _running(true)
    {
        _window = std::make_unique<Window>(Acroy::WindowProps {
            .width      = 1600,
            .height     = 900,
            .fullscreen = false,
            .title      = "Acroy Engine"
        });

        _window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));

        _renderer = std::make_unique<Renderer>(RendererDesc {
            .window = _window.get(),
            .enableVSync = false
        });

        Input::Init(_window.get());
    }

    void Engine::PopLayer(Layer* layer)
    {
        auto it = std::find_if(_layers.begin(), _layers.end(),
            [layer](const std::unique_ptr<Layer>& ptr) {
                return ptr.get() == layer;
            }
        );

        if (it != _layers.end())
        {
            (*it)->OnDetach();
            _layers.erase(it);
        }
    }

    void Engine::OnEvent(Event& event)
    {
        switch (event.GetEventType())
        {
            case EventType::WindowClose:
            {
                _running = false;
                break;
            }

            case EventType::WindowResize:
            {
                auto& e = static_cast<WindowResizeEvent&>(event);
                _renderer->OnWindowResize(e);
                break;
            }

            default: break;
        }

        for (auto& layer : std::views::reverse(_layers)) {
			layer->OnEvent(event);
			if (event.handled)
				break;
		}
    }

    void Engine::Run()
    {
        f32 deltaTime = 0.0f;
        f32 lastFrame = 0.0f;

        while (_running)
        {
            _window->PollEvents();

            f32 currentFrame = _window->GetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            for (const std::unique_ptr<Layer>& layer : _layers) {
                layer->OnUpdate(deltaTime);
            }

            _renderer->BeginFrame();
            
            for (const std::unique_ptr<Layer>& layer : _layers) {
                layer->OnRender(*_renderer);
            }

            _renderer->EndFrame();
            _window->SwapBuffers();
        }
    }

    Engine::~Engine()
    {
        std::cout << "Shutting down engine..." << std::endl;

        for (auto& layer : _layers)
            layer->OnDetach();
        
        _layers.clear();
    }
}