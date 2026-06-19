#include "Types.hpp"
#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include <glad/glad.h>
#include <iostream>

namespace Acroy
{
    FrameBuffer::FrameBuffer(const FrameBufferDesc& desc) : _desc(desc)
    {
        glCreateFramebuffers(1, &_handle);

        std::vector<GLenum> drawBuffers;
        drawBuffers.reserve(desc.colorAttachments.size());

        if (!desc.colorAttachments.empty())
        {
            _size = {
                static_cast<int>(desc.colorAttachments.at(0)->GetDesc().width),
                static_cast<int>(desc.colorAttachments.at(0)->GetDesc().height)
            };
        }

        for (u32 i = 0; i < desc.colorAttachments.size(); ++i)
        {
            GLenum attachment = GL_COLOR_ATTACHMENT0 + i;

            glNamedFramebufferTexture(
                _handle,
                attachment,
                desc.colorAttachments[i]->GetHandle(),
                0
            );

            drawBuffers.push_back(attachment);
        }

        if (desc.depthAttachment)
        {
            const Format fmt = desc.depthAttachment->GetDesc().format;
            GLenum attachmentPoint = GL_DEPTH_ATTACHMENT;

            if (fmt == Format::DEPTH24_STENCIL8 || fmt == Format::DEPTH32F_STENCIL8)
                attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;

            glNamedFramebufferTexture(_handle, attachmentPoint, desc.depthAttachment->GetHandle(), 0);
        }

        if (!drawBuffers.empty())
        {
            glNamedFramebufferDrawBuffers(_handle, drawBuffers.size(), drawBuffers.data());
            glNamedFramebufferReadBuffer(_handle, GL_COLOR_ATTACHMENT0);
        }
        else
        {
            glNamedFramebufferDrawBuffers(_handle, 0, nullptr);
            std::cout << "Framebuffer has no color attachment." << std::endl;
        }

        u32 status = glCheckNamedFramebufferStatus(_handle, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "[FrameBuffer] incomplete: 0x" << std::hex << status << std::dec << std::endl;
        }

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cout << "[FrameBuffer] GL error after creation: 0x" << std::hex << err << std::dec << std::endl;
        }
    }

    void FrameBuffer::Resize(const glm::ivec2& size)
    {
        if (size == _size || size.x == 0 || size.y == 0)
            return;

        _size = size;

        for (Texture* colorAttachment : _desc.colorAttachments)
        {
            colorAttachment->Resize(size);
        }

        if (_desc.depthAttachment)
        {
            _desc.depthAttachment->Resize(size);
        }

        for (u32 i = 0; i < _desc.colorAttachments.size(); ++i)
        {
            GLenum attachment = GL_COLOR_ATTACHMENT0 + i;

            glNamedFramebufferTexture(
                _handle,
                attachment,
                _desc.colorAttachments[i]->GetHandle(),
                0
            );
        }

        if (_desc.depthAttachment)
        {
            const Format fmt = _desc.depthAttachment->GetDesc().format;
            GLenum attachmentPoint = GL_DEPTH_ATTACHMENT;

            if (fmt == Format::DEPTH24_STENCIL8 || fmt == Format::DEPTH32F_STENCIL8)
                attachmentPoint = GL_DEPTH_STENCIL_ATTACHMENT;

            glNamedFramebufferTexture(_handle, attachmentPoint, _desc.depthAttachment->GetHandle(), 0);
        }

        u32 status = glCheckNamedFramebufferStatus(_handle, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "[FrameBuffer] incomplete: 0x" << std::hex << status << std::dec << std::endl;
        }
    }

    void FrameBuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _handle);
    }

    void FrameBuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FrameBuffer::~FrameBuffer()
    {
        glDeleteFramebuffers(1, &_handle);
    }
}