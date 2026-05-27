#pragma once

#include "Types.hpp"
#include <format>

namespace Acroy
{
    enum class EventType : u32
    {
        WindowClose, WindowResize,
        MouseMoved, MouseScrolled, MouseButtonPressed, MouseButtonReleased,
        KeyPressed, KeyReleased
    };

    class Event {
    public:
        bool handled = false;

        virtual ~Event() = default;
        
        virtual std::string ToString()   const = 0;
        virtual EventType GetEventType() const = 0;
    };

    class WindowCloseEvent : public Event {
    public:
        std::string ToString() const override {
            return "WindowCloseEvent";
        }

        EventType GetEventType() const override {
            return EventType::WindowClose;
        }
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(s32 x, s32 y) : _x(x), _y(y) {}

        s32 GetWidth() const { return _x; }
        s32 GetHeight() const { return _y; }

        std::string ToString() const override {
            return std::format("WindowResizeEvent: {}, {}", _x, _y);
        }

        EventType GetEventType() const override {
            return EventType::WindowResize;
        }
    private:
        s32 _x, _y;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(s32 button) : _button(button) {}

        s32 GetButton() const { return _button; }

        std::string ToString() const override {
            return std::format("MouseButtonPressedEvent: {}", _button);
        }

        EventType GetEventType() const override {
            return EventType::MouseButtonPressed;
        }

    protected:
        s32 _button;
    };

    class MouseButtonReleasedEvent : public MouseButtonPressedEvent {
    public:
        MouseButtonReleasedEvent(s32 button) : MouseButtonPressedEvent(button) {}

        std::string ToString() const override {
            return std::format("MouseButtonReleasedEvent: {}", _button);
        }

        EventType GetEventType() const override {
            return EventType::MouseButtonReleased;
        }
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(f32 x, f32 y)
            : _x(x), _y(y) {}

        f32 GetX() const { return _x; }
        f32 GetY() const { return _y; }

        std::string ToString() const override {
            return std::format("MouseMovedEvent: {}, {}", _x, _y);
        }

        EventType GetEventType() const override {
            return EventType::MouseMoved;
        }

    protected:
        f32 _x, _y;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(f32 xOffset, f32 yOffset)
            : _xOffset(xOffset), _yOffset(yOffset) {}

        f32 GetXOffset() const { return _xOffset; }
        f32 GetYOffset() const { return _yOffset; }

        std::string ToString() const override {
            return std::format("MouseScrolledEvent: {}, {}", _xOffset, _yOffset);
        }

        EventType GetEventType() const override {
            return EventType::MouseScrolled;
        }

    private:
        f32 _xOffset, _yOffset;
    };
    
    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(s32 keycode, bool repeat) : _keycode(keycode), _repeat(repeat) {}

        s32 GetKey() const { return _keycode; }
        bool GetRepeat() const { return _repeat; }

        std::string ToString() const override {
            return std::format("KeyPressedEvent: {}, repeat={}", _keycode, _repeat);
        }

        EventType GetEventType() const override {
            return EventType::KeyPressed;
        }

    private:
        s32 _keycode;
        bool _repeat;
    };

    class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(s32 keycode) : _keycode(keycode) {}

        s32 GetKey() const { return _keycode; }

        std::string ToString() const override {
            return std::format("KeyReleasedEvent: {}", _keycode);
        }

        EventType GetEventType() const override {
            return EventType::KeyReleased;
        }

    private:
        s32 _keycode;
    };
}
