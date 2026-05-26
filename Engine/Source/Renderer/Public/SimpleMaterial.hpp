#pragma once

#include "Material.hpp"
#include <memory>

namespace Acroy
{
    class Texture;
    class Sampler;

    class SimpleMaterial : public Material
    {
    public:  
        SimpleMaterial();
        ~SimpleMaterial();

        void SetColor(f32 v[4]);
        void SetTexture(const std::shared_ptr<Texture>& tex);

    private:
        static MaterialDesc CreateDesc();

    private:
        Sampler* _sampler;

        std::shared_ptr<Texture> _texture;
    };

}