#pragma once

#include "Types.hpp"
#include <initializer_list>

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

        u32 GetHandle() const { return m_handle; }

    private:
        u32 m_handle = 0;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram(std::initializer_list<Shader*> shaders);
        ~ShaderProgram();

        u32 GetHandle() const { return m_handle; }

    private:
        u32 m_handle = 0;
    };
}
