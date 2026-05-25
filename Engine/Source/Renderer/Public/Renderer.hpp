#pragma once

#include "Types.hpp"
#include <unordered_map>
#include <vector>

namespace Acroy
{
    class Window;
    class FrameBuffer;
    class Mesh;
    class Material;
    class Shader;
    class GraphicsPipeline;

    struct VertexAttribute;
    enum class PrimitiveType;

    struct RendererDesc
    {
        Window* window;
        bool enableVSync;
    };

    struct PipelineKey
    {
        Shader*          vs;
        Shader*          fs;
        std::vector<VertexAttribute> vertexAttributes;
        PrimitiveType    primitive;
    };

    class PipelineCache
    {
    public:
        GraphicsPipeline* GetOrCreate(const PipelineKey& key);
        ~PipelineCache();

    private:
        std::unordered_map<u64, GraphicsPipeline*> _pipelines;
    };

    class Renderer
    {
    public:
        Renderer(const RendererDesc& desc);
        ~Renderer();

        void BeginFrame();
        void EndFrame();

        void DrawMesh(Mesh* mesh);
        void DrawMesh(Mesh* mesh, Material* mat);

    private:
        Window*        _window;
        FrameBuffer*   _frameBuffer;
        PipelineCache  _pipelineCache;
    };
}