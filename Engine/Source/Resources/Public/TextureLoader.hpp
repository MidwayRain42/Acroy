#pragma once

namespace Acroy {

    class Texture;
    class Sampler;

    class TextureLoader {
    public:
        static Texture* FromFile(const char* path);
    };

    class SamplerFactory {
    public:
        static Sampler* NearestRepeat();
        // static Sampler* LinearRepeat();
    };

}
