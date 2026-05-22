#include "Window.hpp"
#include <GLFW/glfw3.h>
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

    void Window::Update()
    {
        glfwPollEvents();
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