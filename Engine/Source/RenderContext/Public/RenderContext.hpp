#pragma once

#include "Types.hpp"
#include <glm/vec4.hpp>

namespace Acroy
{
    class Window;
    class FrameBuffer;
    class Buffer;
    class VertexArray;
    class Sampler;
    class Texture;
    class ShaderProgram;

    enum class IndexType { UInt32, UInt8 };
    enum class PrimitiveType { TriangleList, LineList, LineStrip, PointList, TriangleStrip };
    enum class DepthFunc { Less, Lequal, Greater, Always };
    enum class BlendFactor { Zero, One, SrcAlpha, OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha };
    enum class CullMode { None, Front, Back };
    enum class FrontFace { CW, CCW };

    struct RenderState
    {
        // Depth
        bool depthTest = true;
        bool depthWrite = true;
        DepthFunc depthFunc = DepthFunc::Less;

        // Blending
        bool blendEnabled = false;
        BlendFactor srcBlend = BlendFactor::One;
        BlendFactor dstBlend = BlendFactor::Zero;

        // Culling
        CullMode cullMode = CullMode::Back;
        FrontFace frontFace = FrontFace::CCW;

        // Output control
        bool colorWrite = true;

        // Debug
        bool wireframe = false;
    };

    class RenderContext
    {
    public:
        static bool Init(Window* window);

        static void Present(FrameBuffer* fb);
        static void SetViewport(s32 x, s32 y, s32 width, s32 height);

        static void Clear(const glm::vec4& color, f32 depth = 1.0f);

        static void ApplyRenderState(const RenderState& state);
        static void BindVertexArray(VertexArray* vertexArray);

        static void BindShaderProgram(ShaderProgram* program);

        static void BindUniformBuffer(u32 slot, const Buffer* buffer, u64 offset = 0, u64 size = 0);
        static void BindTexture(u32 slot, const Texture* texture);
        static void BindSampler(u32 slot, const Sampler* sampler);

        static u64 GetUniformBufferOffsetAlignment();

        static void Draw(u32 vertexCount, PrimitiveType primitiveType = PrimitiveType::TriangleList, u32 firstVertex = 0);
        static void DrawIndexed(u32 indexCount, PrimitiveType primitiveType = PrimitiveType::TriangleList);
    };

}
