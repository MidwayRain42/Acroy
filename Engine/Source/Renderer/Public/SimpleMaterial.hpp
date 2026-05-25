#pragma once

#include "Material.hpp"

namespace Acroy
{
    class Texture;
    class Sampler;

    class SimpleMaterial : public Material
    {
    public:  
        SimpleMaterial();

        void SetColor(f32 v[4]);
        void SetTexture(Texture* tex);

    private:
        static MaterialDesc CreateDesc();

    private:
        Sampler* _sampler;
    };

}