#include "AcroyPCH.hpp"
#include "Application.hpp"
#include "Timestep.hpp"
#include "Log.hpp"
#include "Renderer/Renderer.hpp"
#include <imgui.h>

namespace Acroy
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        s_instance = this;
        
        m_window = std::make_unique<Window>();

        m_imGuiLayer = new ImGuiLayer;
        PushOverlay(m_imGuiLayer);

        m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
        m_window->SetVSync(false);

        Renderer::Init();
    }

    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            if (event.handled)
                break;
            (*it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        m_running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_running)
        {
            float time = m_window->GetTime();
            Timestep ts = time - m_lastFrameTime;
            m_lastFrameTime = time;

            for (Layer* layer : m_layerStack)
            {
                layer->OnUpdate(ts);
            }

            m_imGuiLayer->Begin();
            for (Layer* layer : m_layerStack)
            {
                layer->OnImGuiRender();
            }
            m_imGuiLayer->End();

            m_window->Update();
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowResize(WindowResizeEvent& event)
    {
        Renderer::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
        return false;
    }
}
