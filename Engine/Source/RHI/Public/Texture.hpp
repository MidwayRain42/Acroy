#pragma once

#include "Types.hpp"
#include <glm/vec2.hpp>

namespace Acroy
{
    enum class Format { RGBA8, RGBA16, RGB8, DEPTH_COMPONENT24, DEPTH24_STENCIL8, DEPTH32F_STENCIL8 };
    enum class TextureType { Texture2D };

    struct TextureDesc
    {
        u32 width;
        u32 height;
        s32 mipLevels;
        TextureType type;
        Format format;
    };

    class Texture
    {
    public:
        Texture(const TextureDesc& desc);
        ~Texture();

        TextureDesc GetDesc() const { return _desc; }

        void UploadData(const void* data, u32 size, s32 xOffset = 0, s32 yOffset = 0);

        void Resize(const glm::ivec2& size);

        void Bind(u32 slot) const;

        u32 GetHandle() const { return _handle; }

    private:
        void Invalidate();
        
    private:
        TextureDesc _desc;
        u32 _handle;
    };

    enum class SamplerFilter { Nearest, Linear };
    enum class SamplerWrap { Repeat, ClampToEdge };

    struct SamplerDesc
    {
        SamplerFilter minFilter;
        SamplerFilter magFilter;
        SamplerWrap wrapS;
        SamplerWrap wrapT;
    };

    class Sampler
    {
    public:
        Sampler(const SamplerDesc& desc);
        ~Sampler();

        u32 GetHandle() const { return _handle; }
    
    private:
        SamplerDesc _desc;
        u32 _handle;
    };
}