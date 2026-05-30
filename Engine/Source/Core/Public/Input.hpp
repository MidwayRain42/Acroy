#pragma once

#include "Types.hpp"
#include <glm/vec2.hpp>

namespace Acroy {

    enum class MouseMode { Normal, Hidden, Disabled };

    class Window;

    class Input
    {
    public:
        static bool IsKeyPressed(s32 keycode);
        static bool IsMouseButtonPressed(s32 button);
        static glm::vec2 GetMousePosition();
        static void SetMouseMode(MouseMode mode);

    private:
        static void Init(Window* window);
        friend class Engine;

    private:
        static MouseMode m_mouseMode;
        static Window* m_window;
    };
}
