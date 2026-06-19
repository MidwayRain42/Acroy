#pragma once

#include "Texture.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace Acroy
{
    class ResourceManager
    {
    public:
        static std::shared_ptr<Texture> LoadTexture(std::string_view path);
        static std::shared_ptr<Mesh>    LoadOBJ(std::string_view path);
        static std::shared_ptr<Shader>  LoadShader(std::string_view path, ShaderStage stage);

    private:
        static std::string     LoadFileText(std::string_view path);
        static std::vector<u8> LoadFileBinary(std::string_view path);
    };
}
