#pragma once

#include "Types.hpp"
#include "Transform.hpp"
#include "RenderContext.hpp"

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
    class Skybox;

    struct RendererDesc
    {
        Window* window;
        bool enableVSync;
    };

    struct PerFrameData
    {
        glm::mat4 view;
        glm::mat4 proj;
        float time;
    };

    struct PerObjectData
    {
        glm::mat4 model;
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

        void DrawSkybox(const Skybox& sky);

    private:
        Window*        m_window              = nullptr;
        Buffer*        m_frameUniformBuffer  = nullptr;
        Buffer*        m_objectUniformBuffer = nullptr;
        Camera*        m_cam                 = nullptr;

        PerFrameData   m_perFrameData;
        PerObjectData  m_perObjectData;

        u64            m_objectStride;
        u64            m_objectOffset;

        RenderState    m_renderState;
    };
}