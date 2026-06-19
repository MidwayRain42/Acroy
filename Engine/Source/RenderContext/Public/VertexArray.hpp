#pragma once
#include "Types.hpp"

namespace Acroy
{
    class Buffer;

    enum class Type { Float, Int, UInt };

    struct VertexAttribute
    {
        u32 location = 0;
        u32 componentCount = 0;
        u32 stride = 0;
        u32 offset = 0;
        Type type = Type::Float;
        bool normalized = false;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        // void Bind();
        // void UnBind();

        u32 GetHandle() const { return m_handle; }

        void AddVertexBuffer(
            const Buffer* vertexBuffer,
            const VertexAttribute* vertexAttributes,
            u32 numVertexAttributes,
            u32 binding,
            usize offset,
            usize stride
        );

        void SetIndexBuffer(Buffer* indexBuffer);

    private:
        u32 m_handle = 0;
    };
}