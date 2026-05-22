#include "Texture.hpp"
#include <glad/glad.h>
#include <iostream>

namespace Acroy
{
    static GLenum GetGLFormat(Format format)
    {
        switch (format)
        {
            case Format::RGBA8:              return GL_RGBA8;
            case Format::RGB8:               return GL_RGB8;
            case Format::RGBA16:             return GL_RGBA16;
            case Format::DEPTH_COMPONENT24:  return GL_DEPTH_COMPONENT24;
            case Format::DEPTH24_STENCIL8:   return GL_DEPTH24_STENCIL8;
            case Format::DEPTH32F_STENCIL8:  return GL_DEPTH32F_STENCIL8;
        }

        return GL_RGBA8;
    }

    static GLenum GetGLPixelFormat(Format format)
    {
        switch (format)
        {
            case Format::RGBA8:              return GL_RGBA;
            case Format::RGB8:               return GL_RGB;
            case Format::RGBA16:             return GL_RGBA;
            case Format::DEPTH_COMPONENT24:  return GL_DEPTH_COMPONENT;
            case Format::DEPTH24_STENCIL8:   return GL_DEPTH_STENCIL;
            case Format::DEPTH32F_STENCIL8:  return GL_DEPTH_STENCIL;
        }

        return GL_RGBA;
    }

    static GLenum GetGLTextureType(TextureType type)
    {
        switch (type)
        {
            case TextureType::Texture2D: return GL_TEXTURE_2D;
        }

        return GL_TEXTURE_2D;
    }

    static GLint GetGLSamplerFilter(SamplerFilter filter)
    {
        switch (filter)
        {
            case SamplerFilter::Linear:  return GL_LINEAR;
            case SamplerFilter::Nearest: return GL_NEAREST;
        }

        return GL_NEAREST;
    }

    static GLint GetGLSamplerWrap(SamplerWrap wrap)
    {
        switch (wrap)
        {
            case SamplerWrap::Repeat:      return GL_REPEAT;
            case SamplerWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
        }

        return GL_REPEAT;
    }

    Texture::Texture(const TextureDesc& desc) : _desc(desc)
    {
        Invalidate();
    }

    void Texture::Resize(const glm::ivec2& size)
    {
        _desc.width  = static_cast<u32>(size.x);
        _desc.height = static_cast<u32>(size.y);

        glDeleteTextures(1, &_handle);

        Invalidate();
    }

    void Texture::Invalidate()
    {
        glCreateTextures(GetGLTextureType(_desc.type), 1, &_handle);

        switch (_desc.type)
        {
            case TextureType::Texture2D:
                glTextureStorage2D(_handle, _desc.mipLevels, GetGLFormat(_desc.format), _desc.width, _desc.height);
                break;
            default:
                std::cout << "Unsupported texture type!" << std::endl;
                break;
        }
    }

    void Texture::Bind(u32 slot) const
    {
        glBindTextureUnit(slot, _handle);
    }

    void Texture::UploadData(const void* data, u32 size, s32 xOffset, s32 yOffset)
    {
        switch (_desc.type)
        {
            case TextureType::Texture2D:
            {
                glTextureSubImage2D(
                    _handle,
                    0,
                    xOffset, yOffset,
                    _desc.width,
                    _desc.height,
                    GetGLPixelFormat(_desc.format),
                    GL_UNSIGNED_BYTE,
                    data
                );

                break;
            }
        }

        glGenerateTextureMipmap(_handle);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &_handle);
    }


    Sampler::Sampler(const SamplerDesc& desc) : _desc(desc)
    {
        glCreateSamplers(1, &_handle);

        // glSamplerParameteri(_handle, GL_TEXTURE_MIN_FILTER, GetGLSamplerFilter(_desc.minFilter) == GL_LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);

        glSamplerParameteri(_handle, GL_TEXTURE_MIN_FILTER, GetGLSamplerFilter(_desc.minFilter));
        glSamplerParameteri(_handle, GL_TEXTURE_MAG_FILTER, GetGLSamplerFilter(_desc.magFilter));
        glSamplerParameteri(_handle, GL_TEXTURE_WRAP_S, GetGLSamplerWrap(_desc.wrapS));
        glSamplerParameteri(_handle, GL_TEXTURE_WRAP_T, GetGLSamplerWrap(_desc.wrapT));
    }

    Sampler::~Sampler()
    {
        glDeleteSamplers(1, &_handle);
    }
}
