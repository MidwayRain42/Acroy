#pragma once

#include "Types.hpp"
#include <glm/vec3.hpp>
#include <vector>
#include "VertexArray.hpp"

namespace Acroy
{
    class Buffer;
    class VertexArray;

    enum class PrimitiveType;

    struct VertexLayout
    {
        std::vector<VertexAttribute> attribs;

        static VertexLayout PosColorUV();
        static VertexLayout PosNormalUV();
        // static VertexLayout PosOnly();
    };

    struct MeshData
    {
        f32* vertices;
        u32* indices;

        usize verticesSize;
        u32 indexCount;

        VertexLayout layout;
    };

    class Mesh
    {
    public:        
        Mesh(const MeshData& data, bool isStatic = true);
        ~Mesh();

        VertexArray* GetVertexArray() const { return m_vertexArray; }

        Buffer* GetVertexBuffer() const { return m_vertexBuffer; }
        Buffer* GetIndexBuffer()  const { return m_indexBuffer; }

        u32 GetIndexCount() const { return m_indexCount; }
        inline PrimitiveType GetPrimitiveType() const { return _primitiveType; }

        // const std::vector<VertexAttribute>& GetVertexAttributes() const { return _vertexAttributes; }

    private:
        VertexArray* m_vertexArray;
        Buffer*      m_vertexBuffer;
        Buffer*      m_indexBuffer;
        u32          m_indexCount;

        PrimitiveType _primitiveType;
        // std::vector<VertexAttribute> _vertexAttributes;
    };
}
