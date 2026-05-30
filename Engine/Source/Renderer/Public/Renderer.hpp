#pragma once

#include "Types.hpp"
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>

namespace Acroy
{
    class Window;
    class FrameBuffer;
    class Mesh;
    class Material;
    class Shader;
    class GraphicsPipeline;
    class Camera;
    class Buffer;
    class WindowResizeEvent;

    struct Transform;

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

    struct PerFrameData
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        f32 time;
    };

    class Renderer
    {
    public:
        Renderer(const RendererDesc& desc);
        ~Renderer();

        void OnWindowResize(WindowResizeEvent& event);
        void ResizeFrameBuffer(s32 x, s32 y);
        void BeginFrame();
        void EndFrame();

        void SetCamera(Camera& cam);

        void DrawMesh(Mesh* mesh);
        void DrawMesh(Mesh* mesh, Material* mat, const Transform& transform);

    private:
        Window*        _window;
        FrameBuffer*   _frameBuffer;
        PipelineCache  _pipelineCache;

        Buffer* _perFrameUniform;

        Camera* _cam = nullptr;

        PerFrameData _perFrameData;
    };
}