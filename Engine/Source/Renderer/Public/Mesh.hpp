#pragma once

#include "Types.hpp"

namespace Acroy
{
    struct VertexAttribute;
    class Buffer;

    struct MeshData
    {
        f32* vertices;
        u32* indices;

        usize vertexSize;
        u32 vertexCount;
        u32 indexCount;

        VertexAttribute* vertexAttributes;
        u32 vertexAttributesCount;
    };

    class Mesh
    {
    public:        
        Mesh(const MeshData& data, bool isStatic = true);
        ~Mesh();

        Buffer* GetVertexBuffer() const { return _vertexBuffer; }
        Buffer* GetIndexBuffer()  const { return _indexBuffer; }

        u32 GetIndexCount() const { return _indexCount; }

    private:
        Buffer* _vertexBuffer;
        Buffer* _indexBuffer;
        u32 _indexCount;
    };
}
