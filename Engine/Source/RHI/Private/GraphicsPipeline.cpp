#include "GraphicsPipeline.hpp"
#include "Shader.hpp"
#include <glad/glad.h>
#include <iostream>

namespace Acroy
{
    static const GLenum GetGLType(Type type)
    {
        switch (type)
        {
            case Type::Float: return GL_FLOAT;
            case Type::Int:   return GL_INT;
            case Type::UInt:  return GL_UNSIGNED_INT;
            
            default: return GL_FLOAT;
        }
    }

    GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineDesc& desc) : _desc(desc)
    {
        if (desc.vertexAttributes && desc.numVertexAttributes > 0)
        {
            _vertexAttributes.assign(desc.vertexAttributes, desc.vertexAttributes + desc.numVertexAttributes);
            _desc.vertexAttributes = _vertexAttributes.data();
        }

        _handle = glCreateProgram();

        glAttachShader(_handle, desc.vertexShader->GetHandle());
        glAttachShader(_handle, desc.fragmentShader->GetHandle());

        glLinkProgram(_handle);
        
        GLint success;
        glGetProgramiv(_handle, GL_LINK_STATUS, &success);

        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(_handle, 1024, nullptr, log);
            std::cout << "Program link error: " << log << std::endl;
        }

        glCreateVertexArrays(1, &_vao);

        SetupVertexAttributes(_desc);
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteProgram(_handle);
    }

    void GraphicsPipeline::SetupVertexAttributes(const GraphicsPipelineDesc& desc)
    {
        for (int i{0}; i < desc.numVertexAttributes; ++i)
        {
            const VertexAttribute& attrib = desc.vertexAttributes[i];

            glEnableVertexArrayAttrib(_vao, attrib.location);
            glVertexArrayAttribFormat(_vao, attrib.location, attrib.componentCount, GetGLType(attrib.type), attrib.normalized, attrib.offset);
            glVertexArrayAttribBinding(_vao, attrib.location, 0);
        }

        // glVertexArrayBindingDivisor(_vao, 0, 0);
    }

}