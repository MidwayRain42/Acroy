#pragma once

#include "Types.hpp"
#include <glm/vec4.hpp>

namespace Acroy
{
    class Window;
    class FrameBuffer;
    class Buffer;
    class GraphicsPipeline;
    class Sampler;
    class Texture;

    struct RenderPassDesc
    {
        glm::vec4 clearColor = glm::vec4(0.0f);
        float clearDepth = 1.0f;
        bool clearColorBuffer = true;
        bool clearDepthBuffer = true;
    };

    enum class IndexType { UInt32, UInt8 };

    class RHI
    {
    public:
        static bool Init(Window* window);

        static void BeginRenderPass(FrameBuffer* fb, const RenderPassDesc& desc);
        static void EndRenderPass();
        static void Present(FrameBuffer* fb);

        static void BindGraphicsPipeline(GraphicsPipeline* pipeline);

        static void BindVertexBuffer(u32 slot, const Buffer* buffer, u64 offset = 0);
        static void BindIndexBuffer(const Buffer* buffer, IndexType type = IndexType::UInt32, u64 offset = 0);
        static void BindUniformBuffer(u32 slot, const Buffer* buffer, u64 offset = 0);
        static void BindTexture(u32 slot, const Texture* texture);
        static void BindSampler(u32 slot, const Sampler* sampler);

        static void Draw(u32 vertexCount, u32 firstVertex = 0);
        static void DrawIndexed(u32 indexCount);
    };

}
