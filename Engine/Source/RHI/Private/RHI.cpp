#include "Types.hpp"
#include "RHI.hpp"
#include "Window.hpp"
#include "FrameBuffer.hpp"
#include "GraphicsPipeline.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Acroy
{
    static const GLenum GetGLPrimitiveType(PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::PointList:     return GL_POINTS;
            case PrimitiveType::LineList:      return GL_LINES;
            case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
            case PrimitiveType::TriangleList:  return GL_TRIANGLES;
            case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;

            default: return GL_TRIANGLES;
        }
    }

    namespace
    {
        static FrameBuffer* s_currentFrameBuffer;
        static RenderPassDesc s_currentRenderPassDesc;

        static GraphicsPipeline* s_currentPipeline;

        static IndexType s_currentIndexType;
    }

    bool RHI::Init(Window* window)
    {
        const bool ok = gladLoadGLLoader(window->GetProcAddress());
        if (!ok)
            return false;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        const auto size = window->GetSize();
        
        std::cout << "Vendor:   " << (const char*)glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version:  " << (const char*)glGetString(GL_VERSION) << std::endl;

        glViewport(0, 0, size.x, size.y);

        return true;
    }

    void RHI::BeginRenderPass(FrameBuffer* fb, const RenderPassDesc& desc)
    {
        s_currentRenderPassDesc = desc;
        s_currentFrameBuffer = fb;

        fb->Bind();
        
        const glm::ivec2 size = fb->GetSize();
        glViewport(0, 0, size.x, size.y);

        if (desc.clearColorBuffer)
            glClearNamedFramebufferfv(fb->GetHandle(), GL_COLOR, 0, glm::value_ptr(desc.clearColor));

        if (desc.clearDepthBuffer)
        {
            glClearNamedFramebufferfv(fb->GetHandle(), GL_DEPTH, 0, &desc.clearDepth);

            // glClearNamedFramebufferfi(
            //     fb->GetHandle(),
            //     GL_DEPTH_STENCIL,
            //     0,
            //     desc.clearDepth, 0
            // );
        }
    }

    void RHI::EndRenderPass()
    {
        s_currentFrameBuffer->Unbind();
        s_currentFrameBuffer = nullptr;
    }

    void RHI::Present(FrameBuffer* fb)
    {
        const glm::ivec2 size = fb->GetSize();

        glBlitNamedFramebuffer(
            fb->GetHandle(), 0,
            0, 0, size.x, size.y,
            0, 0, size.x, size.y,
            GL_COLOR_BUFFER_BIT,
            GL_NEAREST
        );
    }

    void RHI::BindGraphicsPipeline(GraphicsPipeline* pipeline)
    {
        s_currentPipeline = pipeline;
        glUseProgram(pipeline->GetHandle());
        glBindVertexArray(pipeline->GetVAOHandle());
    }

    void RHI::Draw(u32 vertexCount, u32 firstVertex)
    {
        if (s_currentPipeline)
        {
            glDrawArrays(GetGLPrimitiveType(s_currentPipeline->GetDesc().primitiveType), firstVertex, vertexCount);
        }
    }

    void RHI::BindVertexBuffer(u32 slot, const Buffer* buffer, u64 offset)
    {
        if (s_currentPipeline)
        {
            glVertexArrayVertexBuffer(
                s_currentPipeline->GetVAOHandle(),
                slot,
                buffer->GetHandle(),
                static_cast<GLintptr>(offset),
                s_currentPipeline->GetDesc().vertexAttributes[slot].stride
            );
        }
    }

    void RHI::BindIndexBuffer(const Buffer* buffer, IndexType type, u64 offset)
    {
        glVertexArrayElementBuffer(s_currentPipeline->GetVAOHandle(), buffer->GetHandle());
        s_currentIndexType = type;
    }

    void RHI::BindUniformBuffer(u32 slot, const Buffer* buffer, u64 offset)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, slot, buffer->GetHandle(), static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(buffer->GetDesc().size));
    }

    void RHI::BindTexture(u32 slot, const Texture* texture)
    {
        glBindTextureUnit(slot, texture->GetHandle());
    }

    void RHI::BindSampler(u32 slot, const Sampler* sampler)
    {
        glBindSampler(slot, sampler->GetHandle());
    }

    void RHI::DrawIndexed(u32 indexCount)
    {
        const GLenum glIndexType = (s_currentIndexType == IndexType::UInt32) ? GL_UNSIGNED_INT : GL_UNSIGNED_BYTE;
        glDrawElements(GetGLPrimitiveType(s_currentPipeline->GetDesc().primitiveType), indexCount, glIndexType, nullptr);
    }
}