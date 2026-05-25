#include "ShaderLoader.hpp"
#include "Shader.hpp"
#include "FileUtils.hpp"

namespace Acroy
{
    Shader* ShaderLoader::FromFile(const char* path, ShaderStage stage)
    {
        std::vector<u8> shaderBinary = LoadFileBinary(path);

        ShaderDesc desc{};
        desc.stage      = stage;
        desc.entryPoint = "main";
        desc.size       = shaderBinary.size();
        desc.binary     = shaderBinary.data();

        return new Shader(desc);
    }

}