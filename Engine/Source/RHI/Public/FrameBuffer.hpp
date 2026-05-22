#pragma once

#include "Types.hpp"
#include <vector>
#include <glm/vec2.hpp>

namespace Acroy
{
    class Texture;

    struct FrameBufferDesc
    {
        std::vector<Texture*> colorAttachments;
        Texture* depthAttachment = nullptr;
    };

    class FrameBuffer
    {
    public:
        FrameBuffer(const FrameBufferDesc& desc);
        ~FrameBuffer();

        void Bind() const;
        void Unbind() const;

        void Resize(const glm::ivec2& size);

        glm::ivec2 GetSize() const { return _size; }
        u32 GetHandle() const { return _handle; }

        FrameBufferDesc GetDesc() const { return _desc; }

    private:
        FrameBufferDesc _desc;
        u32 _handle;
        glm::ivec2 _size;
    };
}