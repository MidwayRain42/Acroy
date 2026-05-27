#include "Window.hpp"
#include <GLFW/glfw3.h>
#include "Events.hpp"
#include <stdexcept>

namespace Acroy
{
    Window::Window(const WindowProps& props) : _props(props)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = glfwCreateWindow(props.width, props.height, props.title, props.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        if (!_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(_window);

        glfwSetWindowUserPointer(_window, &_eventCallback);

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent e;
            eventCallbackFn(e);
        });

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));
            WindowResizeEvent e(width, height);
            eventCallbackFn(e);
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent e(static_cast<f32>(xpos), static_cast<f32>(ypos));
            eventCallbackFn(e);
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent e(static_cast<f32>(xoffset), static_cast<f32>(yoffset));
            eventCallbackFn(e);
        });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));
            
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e(key, false);
                    eventCallbackFn(e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e(key);
                    eventCallbackFn(e);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e(key, true);
                    eventCallbackFn(e);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto& eventCallbackFn = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent e(button);
                    eventCallbackFn(e);
                    break;
                }

                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent e(button);
                    eventCallbackFn(e);
                    break;
                }
            }
        });
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    f32 Window::GetTime() const
    {
        return static_cast<f32>(glfwGetTime());
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(_window);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(_window);
    }

    glm::ivec2 Window::GetSize() const
    {
        int width, height;
        glfwGetFramebufferSize(_window, &width, &height);
        return glm::ivec2(width, height);
    }

    void* Window::GetNativeHandle() const
    {
        return _window;
    }

    Window::ProcAddress Window::GetProcAddress() const
    {
        return reinterpret_cast<Window::ProcAddress>(glfwGetProcAddress);
    }

    void Window::SetFullscreen(bool enabled)
    {
        if (enabled)
        {
            glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, _props.width, _props.height, GLFW_DONT_CARE);
        }
        else
        {
            glfwSetWindowMonitor(_window, nullptr, 0, 0, _props.width, _props.height, GLFW_DONT_CARE);
        }
    }

    bool Window::IsFullscreen() const
    {
        return glfwGetWindowMonitor(_window) != nullptr;
    }
}