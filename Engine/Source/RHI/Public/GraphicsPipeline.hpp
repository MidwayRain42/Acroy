#pragma once

#include "Types.hpp"
#include <vector>

namespace Acroy
{
    class Shader;

    enum class Type { Float, Int, UInt };
    enum class PrimitiveType { PointList, LineList, LineStrip, TriangleList, TriangleStrip };

    struct VertexAttribute
    {
        u32 location = 0;
        u32 componentCount = 0;
        u32 stride = 0;
        u32 offset = 0;
        Type type = Type::Float;
        bool normalized = false;
        const char* debugName = nullptr;
    };

    struct GraphicsPipelineDesc
    {
        Shader* vertexShader = nullptr;
        Shader* fragmentShader = nullptr;

        std::vector<VertexAttribute> vertexAttributes;

        PrimitiveType primitiveType = PrimitiveType::TriangleList;
    };

    class GraphicsPipeline
    {
    public:
        GraphicsPipeline(const GraphicsPipelineDesc& desc);
        ~GraphicsPipeline();

        const GraphicsPipelineDesc& GetDesc() const { return _desc; }
        
    private:
        void SetupVertexAttributes(const GraphicsPipelineDesc& desc);
        
        u32 GetHandle() const { return _handle; }
        u32 GetVAOHandle() const { return _vao; }

        friend class RHI;

    private:
        GraphicsPipelineDesc _desc;
        u32 _handle;
        u32 _vao;
    };
}