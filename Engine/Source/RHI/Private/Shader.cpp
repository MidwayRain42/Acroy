#include "Shader.hpp"
#include <glad/glad.h>
#include <iostream>

namespace Acroy
{
    static const GLenum ShaderStageToGLenum(ShaderStage stage)
    {
        switch (stage)
        {
            case ShaderStage::Vertex:   return GL_VERTEX_SHADER;
            case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
            case ShaderStage::Compute:  return GL_COMPUTE_SHADER;

            default: return 0;
        }
    }

    Shader::Shader(const ShaderDesc& desc)
    {
        _handle = glCreateShader(ShaderStageToGLenum(desc.stage));

        glShaderBinary(1, &_handle, GL_SHADER_BINARY_FORMAT_SPIR_V, desc.binary, desc.size);
        glSpecializeShader(_handle, desc.entryPoint, 0, nullptr, nullptr);

    }

    Shader::~Shader()
    {
        glDeleteShader(_handle);
    }
}