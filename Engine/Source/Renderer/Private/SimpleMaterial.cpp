#include "SimpleMaterial.hpp"
#include "Shader.hpp"
#include "FileUtils.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"

namespace Acroy
{
    SimpleMaterial::SimpleMaterial() : Material(CreateDesc())
    {
        _sampler = SamplerFactory::NearestRepeat();
    }

    void SimpleMaterial::SetColor(f32 v[4])
    {
        SetParamFloat4("color", v[0], v[1], v[2], v[3]);
    }

    void SimpleMaterial::SetTexture(Texture* tex)
    {
        SetParamTexture("texture", tex);
        SetParamSampler("texture", _sampler);
        SetParamInt("useTexture", 1);
    }

    MaterialDesc SimpleMaterial::CreateDesc()
    {
        std::vector<u8> vsBinary = LoadFileBinary("/home/sam/Projects/Acroy/Bin/Shaders/vertex.spv");
        std::vector<u8> fsBinary = LoadFileBinary("/home/sam/Projects/Acroy/Bin/Shaders/fragment.spv");

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

        Shader* vs = new Shader(vsDesc);
        Shader* fs = new Shader(fsDesc);

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