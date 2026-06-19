#include "Mesh.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "RenderContext.hpp"

namespace Acroy
{
    Mesh::Mesh(const MeshData& data, bool isStatic)
    {
        BufferDesc vertexBufferDesc{};

        vertexBufferDesc.cpuWritable = !isStatic;
        vertexBufferDesc.persistentMap = !isStatic;

        vertexBufferDesc.size = data.verticesSize;
        vertexBufferDesc.type = BufferType::Vertex;
        vertexBufferDesc.usage = BufferUsage::Static;

        BufferDesc indexBufferDesc{};

        indexBufferDesc.cpuWritable = false;
        indexBufferDesc.persistentMap = false;
        
        indexBufferDesc.size = data.indexCount * sizeof(u32);
        indexBufferDesc.type = BufferType::Index;
        indexBufferDesc.usage = BufferUsage::Static;
        

        m_vertexBuffer = new Buffer(vertexBufferDesc);

        m_vertexBuffer->UploadData(
            static_cast<const void*>(data.vertices),
            data.verticesSize
        );

        m_indexBuffer = new Buffer(indexBufferDesc);

        m_indexBuffer->UploadData(
            static_cast<const void*>(data.indices),
            data.indexCount * sizeof(u32)
        );

        m_indexCount = data.indexCount;

        m_vertexArray = new VertexArray();

        m_vertexArray->AddVertexBuffer(
            m_vertexBuffer,
            data.layout.attribs.data(),
            data.layout.attribs.size(),
            0, 0, data.layout.attribs.at(0).stride
        );

        m_vertexArray->SetIndexBuffer(m_indexBuffer);
        // m_vertexAttributes = data.layout.attribs;
        _primitiveType    = PrimitiveType::TriangleList;
    }

    Mesh::~Mesh()
    {
        delete m_vertexArray;
        delete m_vertexBuffer;
        delete m_indexBuffer;
    }

    VertexLayout VertexLayout::PosColorUV()
    {
        std::vector<VertexAttribute> vertexAttributes(3);

        vertexAttributes[0].location = 0;
        vertexAttributes[0].componentCount = 3;
        vertexAttributes[0].stride = sizeof(float) * 8;
        vertexAttributes[0].offset = 0;
        vertexAttributes[0].type = Type::Float;

        vertexAttributes[1].location = 1;
        vertexAttributes[1].componentCount = 3;
        vertexAttributes[1].stride = sizeof(float) * 8;
        vertexAttributes[1].offset = sizeof(float) * 3;
        vertexAttributes[1].type = Type::Float;

        vertexAttributes[2].location = 2;
        vertexAttributes[2].componentCount = 2;
        vertexAttributes[2].stride = sizeof(float) * 8;
        vertexAttributes[2].offset = sizeof(float) * 6;
        vertexAttributes[2].type = Type::Float;

        return { vertexAttributes };
    }

    VertexLayout VertexLayout::PosNormalUV()
    {
        // Same Layout as PosColorUV
        VertexLayout layout = PosColorUV();

        return layout;
    }
}