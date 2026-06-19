#pragma once

#include "Types.hpp"
#include "Transform.hpp"

#include <glm/mat4x4.hpp>
#include <vector>

namespace Acroy
{
    class Window;
    class Mesh;
    class Material;
    class Camera;
    class Buffer;
    class WindowResizeEvent;

    struct RendererDesc
    {
        Window* window;
        bool enableVSync;
    };

    struct PerFrameData
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        f32 time;
        f32 padding[3];
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

        void DrawMesh(const Mesh& mesh);
        void DrawMesh(const Mesh& mesh, const Material& mat, const Transform& transform);

    private:
        Window*        m_window              = nullptr;
        Buffer*        m_perFrameUniform     = nullptr;
        Buffer*        m_lightingDataUniform = nullptr;
        Camera*        m_cam                 = nullptr;
        PerFrameData   m_perFrameData;
        u64            m_perFrameDataStride  = 0;
        u64            m_perFrameDataOffset  = 0;
    };
}