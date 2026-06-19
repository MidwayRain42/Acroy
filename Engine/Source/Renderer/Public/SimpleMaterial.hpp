#pragma once

#include "Material.hpp"
#include <memory>

namespace Acroy
{
    class Texture;
    class Sampler;
    class Shader;

    class SimpleMaterial : public Material
    {
    public:  
        SimpleMaterial();
        ~SimpleMaterial() = default;

        void SetUVScale(const glm::vec2& scale);
        void SetColor(const glm::vec4& color);
        void SetTexture(Texture* tex);

    private:
        MaterialDesc CreateDesc();

    private:
        std::unique_ptr<Sampler> m_sampler;
        std::shared_ptr<Shader> m_vs;
        std::shared_ptr<Shader> m_fs;
    };

}