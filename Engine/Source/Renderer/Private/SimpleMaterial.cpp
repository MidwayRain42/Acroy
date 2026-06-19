#include "SimpleMaterial.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"

namespace Acroy
{
    SimpleMaterial::SimpleMaterial()
    {
        SamplerDesc sd{};
        sd.magFilter = SamplerFilter::Linear;
        sd.minFilter = SamplerFilter::Linear;
        sd.wrapS     = SamplerWrap::Repeat;
        sd.wrapT     = SamplerWrap::Repeat;
        
        m_sampler = std::make_unique<Sampler>(sd);

        SetDescription(CreateDesc());
    }

    void SimpleMaterial::SetUVScale(const glm::vec2& scale)
    {
        SetParamVector2("uvScale", scale);
    }

    void SimpleMaterial::SetColor(const glm::vec4& color)
    {
        SetParamVector4("color", color);
    }

    void SimpleMaterial::SetTexture(Texture* tex)
    {
        SetParamTexture("texture", tex);
        SetParamSampler("texture", m_sampler.get());
        SetParamInt("useTexture", 1);
    }

    MaterialDesc SimpleMaterial::CreateDesc()
    {
        m_vs = ResourceManager::LoadShader("Bin/Shaders/SimpleVert.spv", ShaderStage::Vertex);
        m_fs = ResourceManager::LoadShader("Bin/Shaders/SimpleFrag.spv", ShaderStage::Fragment);

        MaterialDesc desc{};
        desc.vs = m_vs.get();
        desc.fs = m_fs.get();

        desc.params = {
            { "color",      MaterialParamType::Float4  },
            { "texture",    MaterialParamType::Texture },
            { "useTexture", MaterialParamType::Int     },
            { "uvScale",    MaterialParamType::Float2  }
        };

        return desc;
    }
}