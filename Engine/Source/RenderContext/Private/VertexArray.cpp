#include "VertexArray.hpp"
#include "Buffer.hpp"
#include <glad/glad.h>

namespace Acroy
{
    static const GLenum GetGLType(Type type)
    {
        switch (type)
        {
            case Type::Float: return GL_FLOAT;
            case Type::Int:   return GL_INT;
            case Type::UInt:  return GL_UNSIGNED_INT;
            
            default: return GL_FLOAT;
        }
    }

    VertexArray::VertexArray()
    {
        glCreateVertexArrays(1, &m_handle);
    }

    VertexArray::~VertexArray()
    {
        if (m_handle)
            glDeleteVertexArrays(1, &m_handle);
    }

    void VertexArray::AddVertexBuffer(
        const Buffer* vertexBuffer,
        const VertexAttribute* vertexAttributes,
        u32 numVertexAttributes,
        u32 binding,
        usize offset,
        usize stride)
    {
        glVertexArrayVertexBuffer(
            m_handle,
            binding,
            vertexBuffer->GetHandle(),
            offset,
            stride
        );

        for (u32 i{0}; i < numVertexAttributes; ++i)
        {
            const VertexAttribute& attrib = vertexAttributes[i];

            glEnableVertexArrayAttrib(m_handle, attrib.location);
            
            glVertexArrayAttribFormat(
                m_handle,
                attrib.location,
                attrib.componentCount,
                GetGLType(attrib.type),
                attrib.normalized,
                attrib.offset
            );

            glVertexArrayAttribBinding(m_handle, attrib.location, binding);
        }
    }

    void VertexArray::SetIndexBuffer(Buffer* indexBuffer)
    {
        glVertexArrayElementBuffer(m_handle, indexBuffer->GetHandle());
    }
}