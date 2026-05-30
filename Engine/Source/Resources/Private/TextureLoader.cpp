#include "TextureLoader.hpp"
#include "Texture.hpp"
#include "FileUtils.hpp"

#include "stb_image.h"

#include <cassert>

namespace Acroy
{
    Texture* TextureLoader::FromFile(const char* path)
    {
        stbi_set_flip_vertically_on_load(true);

        TextureDesc desc{};

        desc.mipLevels = 1;
        desc.type      = TextureType::Texture2D;

        s32 x, y, channels;
        u8* bytes = stbi_load(path, &x, &y, &channels, 0);

        if (!bytes)
        {
            printf("Failed to load image: %s\n", stbi_failure_reason());
            assert(false);
        }

        Format format;

        switch (channels)  
        {
            case 3: format = Format::RGB8;  break;
            case 4: format = Format::RGBA8; break;

            default: assert(false && "Unknown Texture Type");
        }

        desc.format = format;
        desc.width  = x;
        desc.height = y;

        usize size = x * y * channels * sizeof(u8);

        Texture* tex = new Texture(desc);
        tex->UploadData(bytes, size);

        stbi_image_free(bytes);

        return tex;
    }

    Sampler* SamplerFactory::NearestRepeat()
    {
        SamplerDesc desc{};
        desc.magFilter = SamplerFilter::Nearest;
        desc.minFilter = SamplerFilter::Nearest;
        desc.wrapS = SamplerWrap::Repeat;
        desc.wrapT = SamplerWrap::Repeat;

        return new Sampler(desc);
    }

    Sampler* SamplerFactory::LinearRepeat()
    {
        SamplerDesc desc{};
        desc.magFilter = SamplerFilter::Linear;
        desc.minFilter = SamplerFilter::Linear;
        desc.wrapS = SamplerWrap::Repeat;
        desc.wrapT = SamplerWrap::Repeat;

        return new Sampler(desc);
    }
}