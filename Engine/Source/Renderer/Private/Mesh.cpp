#include "Mesh.hpp"
#include "Buffer.hpp"
#include "GraphicsPipeline.hpp"

namespace Acroy
{
    Mesh::Mesh(const MeshData& data, bool isStatic)
    {
        BufferDesc vertexBufferDesc{};

        vertexBufferDesc.cpuWritable = !isStatic;
        vertexBufferDesc.persistentMap = !isStatic;

        vertexBufferDesc.size = data.vertexSize;
        vertexBufferDesc.type = BufferType::Vertex;
        vertexBufferDesc.usage = BufferUsage::Static;

        BufferDesc indexBufferDesc{};

        indexBufferDesc.cpuWritable = false;
        indexBufferDesc.persistentMap = false;
        
        indexBufferDesc.size = data.indexCount * sizeof(u32);
        indexBufferDesc.type = BufferType::Index;
        indexBufferDesc.usage = BufferUsage::Static;
        

        _vertexBuffer = new Buffer(vertexBufferDesc);

        _vertexBuffer->UploadData(
            static_cast<const void*>(data.vertices),
            data.vertexSize
        );

        _indexBuffer = new Buffer(indexBufferDesc);

        _indexBuffer->UploadData(
            static_cast<const void*>(data.indices),
            data.indexCount * sizeof(u32)
        );

        _indexCount = data.indexCount;
    }

    Mesh::~Mesh()
    {
        delete _vertexBuffer;
        delete _indexBuffer;
    }
}