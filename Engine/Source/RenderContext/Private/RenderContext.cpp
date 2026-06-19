#include "Types.hpp"
#include "RenderContext.hpp"
#include "Window.hpp"
#include "FrameBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
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

    bool RenderContext::Init(Window* window)
    {
        const bool ok = gladLoadGLLoader(window->GetProcAddress());
        if (!ok)
            return false;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        const auto size = window->GetSize();
        
        std::cout << "Vendor:   " << (const char*)glGetString(GL_VENDOR)   << std::endl;
        std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version:  " << (const char*)glGetString(GL_VERSION)  << std::endl;

        glViewport(0, 0, size.x, size.y);

        return true;
    }

    void RenderContext::Clear(const glm::vec4& color, f32 depth)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClearDepth(depth);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderContext::SetViewport(s32 x, s32 y, s32 width, s32 height)
    {
        glViewport(x, y, width, height);
    }

    void RenderContext::BindVertexArray(VertexArray* vertexArray)
    {
        glBindVertexArray(vertexArray->GetHandle());
    }

    void RenderContext::BindShaderProgram(ShaderProgram* program)
    {
        glUseProgram(program->GetHandle());
    }

    void RenderContext::Draw(u32 vertexCount, PrimitiveType primitiveType, u32 firstVertex)
    {
        glDrawArrays(GetGLPrimitiveType(primitiveType), firstVertex, vertexCount);
    }

    u64 RenderContext::GetUniformBufferOffsetAlignment()
    {
        GLint alignment = 256;
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
        return static_cast<u64>(alignment);
    }

    void RenderContext::BindUniformBuffer(u32 slot, const Buffer* buffer, u64 offset, u64 size)
    {
        if (size == 0)
            size = buffer->GetDesc().size;

        glBindBufferRange(GL_UNIFORM_BUFFER, slot, buffer->GetHandle(), static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size));
    }

    void RenderContext::BindTexture(u32 slot, const Texture* texture)
    {
        glBindTextureUnit(slot, texture->GetHandle());
    }

    void RenderContext::BindSampler(u32 slot, const Sampler* sampler)
    {
        glBindSampler(slot, sampler->GetHandle());
    }

    void RenderContext::DrawIndexed(u32 indexCount, PrimitiveType primitiveType)
    {
        glDrawElements(GetGLPrimitiveType(primitiveType), indexCount, GL_UNSIGNED_INT, nullptr);
    }
}