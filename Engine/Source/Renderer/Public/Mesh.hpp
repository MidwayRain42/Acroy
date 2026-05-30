#pragma once

#include "Types.hpp"
#include <glm/vec3.hpp>
#include "GraphicsPipeline.hpp"

namespace Acroy
{
    struct VertexAttribute;
    class Buffer;

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

        inline Buffer* GetVertexBuffer() const { return _vertexBuffer; }
        inline Buffer* GetIndexBuffer()  const { return _indexBuffer; }

        inline u32 GetIndexCount() const { return _indexCount; }
        inline PrimitiveType GetPrimitiveType() const { return _primitiveType; }

        inline const std::vector<VertexAttribute>& GetVertexAttributes() const { return _vertexAttributes; }

    private:
        Buffer* _vertexBuffer;
        Buffer* _indexBuffer;
        u32     _indexCount;

        PrimitiveType _primitiveType;
        std::vector<VertexAttribute> _vertexAttributes;
    };
}
