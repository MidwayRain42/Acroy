#include "AcroyPCH.hpp"
#include "Renderer/Buffer.hpp"
#include "Core/Log.hpp"
#include "glad/glad.h"

namespace Acroy
{
    VertexBuffer::VertexBuffer(const void* vertices, uint32_t size)
    {
        // glCreateBuffers(1, &m_rendererID);
        // glNamedBufferData(m_rendererID, size, vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void VertexBuffer::UnBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }



    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) : m_indicesCount(count)
    {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void IndexBuffer::UnBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }
}
