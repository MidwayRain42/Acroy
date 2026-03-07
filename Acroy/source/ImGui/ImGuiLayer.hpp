#pragma once

#include "Core/Layer.hpp"
#include "Core/Window.hpp"
#include "Core/Core.hpp"

namespace Acroy
{
    class ImGuiLayer : public Layer
    {
    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
        void SetupStyle();
    };
}
