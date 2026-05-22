#pragma once

#include "Types.hpp"

namespace Acroy
{
    enum class ShaderStage { Vertex, Fragment, Compute };

    struct ShaderDesc
    {
        usize size;
        const void* binary;
        const char* entryPoint;
        ShaderStage stage;
    };

    class Shader
    {
    public:
        Shader(const ShaderDesc& desc);
        ~Shader();

        u32 GetHandle() const { return _handle; }

    private:
        u32 _handle;
    };
}
