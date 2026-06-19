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
        m_handle = glCreateShader(ShaderStageToGLenum(desc.stage));

        glShaderBinary(1, &m_handle, GL_SHADER_BINARY_FORMAT_SPIR_V, desc.binary, desc.size);
        glSpecializeShader(m_handle, desc.entryPoint, 0, nullptr, nullptr);

    }

    Shader::~Shader()
    {
        if (m_handle)
            glDeleteShader(m_handle);
    }

    ShaderProgram::ShaderProgram(std::initializer_list<Shader*> shaders)
    {
        m_handle = glCreateProgram();

        for (Shader* s : shaders)
            glAttachShader(m_handle, s->GetHandle());
        
        glLinkProgram(m_handle);
        
        GLint success;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(m_handle, 1024, nullptr, log);
            std::cout << "Program link error: " << log << std::endl;
        }
    }

    ShaderProgram::~ShaderProgram()
    {
        if (m_handle)
            glDeleteProgram(m_handle);
    }
}