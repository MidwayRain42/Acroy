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

        void SetColor(const glm::vec4& color);
        void SetTexture(Texture* tex);

    private:
        static MaterialDesc CreateDesc();

    private:
        Sampler* _sampler;
    };

}