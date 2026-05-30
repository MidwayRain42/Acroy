#include "Engine.hpp"
#include "Input.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace Acroy {
    
    MouseMode Input::m_mouseMode = MouseMode::Normal;
    Window* Input::m_window = nullptr;

    void Input::Init(Window* window)
    {
        m_window = window;
    }

    bool Input::IsKeyPressed(int keycode)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetNativeHandle());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetNativeHandle());
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetNativeHandle());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

    void Input::SetMouseMode(MouseMode mode)
    {
        if (mode == m_mouseMode)
            return;

        int glfwMode = GLFW_CURSOR_NORMAL;

        switch (mode)
        {
            case MouseMode::Normal:    glfwMode = GLFW_CURSOR_NORMAL;   break;
            case MouseMode::Hidden:    glfwMode = GLFW_CURSOR_HIDDEN;   break;
            case MouseMode::Disabled:  glfwMode = GLFW_CURSOR_DISABLED; break;
        }

        GLFWwindow* window = static_cast<GLFWwindow*>(m_window->GetNativeHandle());
        glfwSetInputMode(window, GLFW_CURSOR, glfwMode);
        m_mouseMode = mode;
    }
}
