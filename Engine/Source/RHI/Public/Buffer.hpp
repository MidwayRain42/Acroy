#pragma once

#include "Types.hpp"

namespace Acroy
{
    enum class BufferType  { Vertex, Index, Uniform, Storage };
    enum class BufferUsage { Static, Dynamic, Stream };
    enum class MapAccess   { Read, Write, ReadWrite };

    struct BufferDesc
    {
        usize size = 0;

        BufferType type;
        BufferUsage usage;

        bool cpuWritable = false;
        bool persistentMap = false;
    };

    class Buffer
    {
    public:
        Buffer(const BufferDesc& desc);
        ~Buffer();

        void UploadData(const void* data, usize size, usize offset = 0);

        void* Map(MapAccess access, usize offset = 0, usize length = 0);
        void Unmap();

        u32 GetHandle() const { return _handle; }
        const BufferDesc& GetDesc() const { return _desc; }
    
    private:
        u32 _handle;
        BufferDesc _desc;
        void* _mappedPtr = nullptr;
    };
}