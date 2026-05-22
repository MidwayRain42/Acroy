#pragma once

#include "Types.hpp"
#include <glm/vec2.hpp>

typedef struct GLFWwindow GLFWwindow;

namespace Acroy
{
    struct WindowProps
    {
        u32 width;
        u32 height;
        bool fullscreen;
        const char *title;
    };

    class Window
    {
    public:
        using ProcAddress = void*(*)(const char*);

        Window(const WindowProps& props);
        ~Window();

        void Update();

        bool ShouldClose() const;

        void SetVSync(bool enabled);

        void SetFullscreen(bool enabled);
        bool IsFullscreen() const;

        f32 GetTime() const;

        glm::ivec2 GetSize() const;

        void* GetNativeHandle() const;
        ProcAddress GetProcAddress() const;

    private:
        GLFWwindow* _window;
        WindowProps _props;
    };
}