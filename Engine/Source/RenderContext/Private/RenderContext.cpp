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
    static const GLenum ToGL(PrimitiveType type)
    {
        switch (type)
        {
            case PrimitiveType::TriangleList:  return GL_TRIANGLES;
            case PrimitiveType::LineList:      return GL_LINES;
            case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
            case PrimitiveType::PointList:     return GL_POINTS;
            case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
        }
        return GL_TRIANGLES;
    }

    static const GLenum ToGL(IndexType type)
    {
        switch (type)
        {
            case IndexType::UInt8:  return GL_UNSIGNED_BYTE;
            case IndexType::UInt32: return GL_UNSIGNED_INT;
        }
        return GL_UNSIGNED_INT;
    }

    static const GLenum ToGL(DepthFunc func)
    {
        switch (func)
        {
            case DepthFunc::Less:    return GL_LESS;
            case DepthFunc::Lequal:  return GL_LEQUAL;
            case DepthFunc::Greater: return GL_GREATER;
            case DepthFunc::Always:  return GL_ALWAYS;
        }
        return GL_LESS;
    }

    static const GLenum ToGL(BlendFactor factor)
    {
        switch (factor)
        {
            case BlendFactor::Zero:              return GL_ZERO;
            case BlendFactor::One:               return GL_ONE;
            case BlendFactor::SrcAlpha:          return GL_SRC_ALPHA;
            case BlendFactor::OneMinusSrcAlpha:  return GL_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::DstAlpha:          return GL_DST_ALPHA;
            case BlendFactor::OneMinusDstAlpha:  return GL_ONE_MINUS_DST_ALPHA;
        }
        return GL_ONE;
    }

    static const GLenum ToGL(CullMode mode)
    {
        switch (mode)
        {
            case CullMode::Front: return GL_FRONT;
            case CullMode::Back:  return GL_BACK;
            case CullMode::None:  return GL_BACK;
        }
        return GL_BACK;
    }

    bool RenderContext::Init(Window* window)
    {
        const bool ok = gladLoadGLLoader(window->GetProcAddress());
        if (!ok)
            return false;

        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        const auto size = window->GetSize();
        
        std::cout << "Vendor:   " << (const char*)glGetString(GL_VENDOR)   << std::endl;
        std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version:  " << (const char*)glGetString(GL_VERSION)  << std::endl;

        glViewport(0, 0, size.x, size.y);

        return true;
    }

    void RenderContext::ApplyRenderState(const RenderState& state)
    {
        // Depth test
        if (state.depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        glDepthFunc(ToGL(state.depthFunc));
        glDepthMask(state.depthWrite ? GL_TRUE : GL_FALSE);

        // Blending
        if (state.blendEnabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);

        glBlendFunc(ToGL(state.srcBlend), ToGL(state.dstBlend));

        // Culling
        if (state.cullMode == CullMode::None)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
            glCullFace(ToGL(state.cullMode));
        }

        glFrontFace(state.frontFace == FrontFace::CCW ? GL_CCW : GL_CW);

        // Color write
        glColorMask(state.colorWrite, state.colorWrite, state.colorWrite, state.colorWrite);

        // Wireframe
        glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);
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
        glDrawArrays(ToGL(primitiveType), firstVertex, vertexCount);
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
        glDrawElements(ToGL(primitiveType), indexCount, GL_UNSIGNED_INT, nullptr);
    }
}