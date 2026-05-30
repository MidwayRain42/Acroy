#include "SimpleMaterial.hpp"
#include "Shader.hpp"
#include "FileUtils.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

namespace Acroy
{
    SimpleMaterial::SimpleMaterial() : Material(CreateDesc())
    {
        _sampler = SamplerFactory::LinearRepeat();
    }

    SimpleMaterial::~SimpleMaterial()
    {
        delete _sampler;
    }

    void SimpleMaterial::SetColor(const glm::vec4& color)
    {
        SetParamVector4("color", color);
    }

    void SimpleMaterial::SetTexture(Texture* tex)
    {
        SetParamTexture("texture", tex);
        SetParamSampler("texture", _sampler);
        SetParamInt("useTexture", 1);
    }

    MaterialDesc SimpleMaterial::CreateDesc()
    {
        static std::vector<u8> vsBinary = LoadFileBinary("Bin/Shaders/vertex.spv");
        static std::vector<u8> fsBinary = LoadFileBinary("Bin/Shaders/fragment.spv");

        ShaderDesc vsDesc{};
        vsDesc.entryPoint = "main";
        vsDesc.stage      = ShaderStage::Vertex;
        vsDesc.size       = vsBinary.size();
        vsDesc.binary     = vsBinary.data();

        ShaderDesc fsDesc{};
        fsDesc.entryPoint = "main";
        fsDesc.stage      = ShaderStage::Fragment;
        fsDesc.size       = fsBinary.size();
        fsDesc.binary     = fsBinary.data();

        static Shader* vs = new Shader(vsDesc);
        static Shader* fs = new Shader(fsDesc);

        MaterialDesc desc{};
        desc.vs = vs;
        desc.fs = fs;

        static MaterialParam params [3]{};
        params[0].name = "color";
        params[0].type = MaterialParamType::Float4;

        params[1].name = "texture";
        params[1].type = MaterialParamType::Texture;

        params[2].name = "useTexture";
        params[2].type = MaterialParamType::Int;

        desc.paramCount = 3;
        desc.params     = params;

        return desc;
    }
}