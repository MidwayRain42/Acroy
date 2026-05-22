#include "Types.hpp"
#include "Buffer.hpp"
#include <glad/glad.h>
#include <cstring>
#include <stdexcept>
#include <cassert>

namespace Acroy
{
    Buffer::Buffer(const BufferDesc& desc) : _desc(desc)
    {
        glCreateBuffers(1, &_handle);

        GLbitfield storageFlags = 0;

        storageFlags |= GL_DYNAMIC_STORAGE_BIT;

        if (_desc.cpuWritable)
        {
            storageFlags |= GL_MAP_WRITE_BIT;
        }

        if (_desc.persistentMap)
        {
            storageFlags |=
                GL_MAP_WRITE_BIT |
                GL_MAP_PERSISTENT_BIT |
                GL_MAP_COHERENT_BIT;
        }

        glNamedBufferStorage(_handle, static_cast<GLsizeiptr>(_desc.size), nullptr, storageFlags);

        if (_desc.persistentMap)
        {
            GLbitfield mapFlags =
                GL_MAP_WRITE_BIT |
                GL_MAP_PERSISTENT_BIT |
                GL_MAP_COHERENT_BIT;

            _mappedPtr = glMapNamedBufferRange(_handle, 0, static_cast<GLsizeiptr>(_desc.size), mapFlags);

            if (!_mappedPtr)
            {
                throw std::runtime_error("Failed to persistently map buffer.");
            }
        }
    }

    void Buffer::UploadData(const void* data, usize size, usize offset)
    {
        assert(offset + size <= _desc.size);

        if (_desc.persistentMap)
        {
            std::memcpy(static_cast<u8*>(_mappedPtr) + offset, data, size);
        }
        else
        {
            glNamedBufferSubData(_handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
        }
    }

    void* Buffer::Map(MapAccess access, usize offset, usize length)
    {
        if (_desc.persistentMap)
            return _mappedPtr;

        if (length == 0)
            length = _desc.size - offset;

        GLbitfield flags = 0;

        switch (access)
        {
            case MapAccess::Read:      flags = GL_MAP_READ_BIT; break;
            case MapAccess::Write:     flags = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT; break;
            case MapAccess::ReadWrite: flags = GL_MAP_READ_BIT  | GL_MAP_WRITE_BIT; break;
        }

        return glMapNamedBufferRange(_handle, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(length), flags);
    }

    void Buffer::Unmap()
    {
        if (_desc.persistentMap)
            return;

        glUnmapNamedBuffer(_handle);
    }

    Buffer::~Buffer()
    {
        if (_mappedPtr)
        {
            glUnmapNamedBuffer(_handle);
        }

        if (_handle)
        {
            glDeleteBuffers(1, &_handle);
        }
    }
}