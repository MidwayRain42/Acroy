#include "Renderer.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "RenderContext.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Events.hpp"
#include "Camera.hpp"

#include <cassert>
#include <iostream>
#include <unordered_map>

namespace Acroy
{
    static u64 AlignUp(u64 value, u64 alignment)
    {
        return (value + alignment - 1) / alignment * alignment;
    }

    Renderer::Renderer(const RendererDesc& desc) : m_window(desc.window)
    {
        std::cout << "Initializing Renderer" << std::endl;

        RenderContext::Init(desc.window);
        desc.window->SetVSync(desc.enableVSync);

        const u64 uboAlignment = RenderContext::GetUniformBufferOffsetAlignment();
        m_perFrameDataStride = AlignUp(sizeof(PerFrameData), uboAlignment);
        const u64 perFrameBufferSize = m_perFrameDataStride * 128;

        BufferDesc perFrameUBODesc{};
        perFrameUBODesc.cpuWritable = true;
        perFrameUBODesc.persistentMap = true;
        perFrameUBODesc.type = BufferType::Uniform;
        perFrameUBODesc.usage = BufferUsage::Dynamic;
        perFrameUBODesc.size = static_cast<u32>(perFrameBufferSize);

        m_perFrameUniform = new Buffer(perFrameUBODesc);
        m_perFrameDataOffset = 0;
    }

    void Renderer::BeginFrame()
    {
        RenderContext::Clear({});

        if (m_cam)
        {
            m_perFrameData.view = glm::inverse(m_cam->GetTransform().GetMatrix());
            m_perFrameData.proj = m_cam->GetProj();
        }
        
        m_perFrameData.time = m_window->GetTime();
        m_perFrameDataOffset = 0;
    }

    void Renderer::EndFrame()
    {
    }

    void Renderer::DrawMesh(const Mesh& mesh)
    {
        RenderContext::BindVertexArray(mesh.GetVertexArray());
        RenderContext::DrawIndexed(mesh.GetIndexCount(), mesh.GetPrimitiveType());
    }

    void Renderer::DrawMesh(const Mesh& mesh, const Material& mat, const Transform& transform)
    {
        RenderContext::BindShaderProgram(mat.GetProgram());

        m_perFrameData.model = transform.GetMatrix();

        m_perFrameUniform->UploadData(&m_perFrameData, sizeof(PerFrameData), m_perFrameDataOffset);
        RenderContext::BindUniformBuffer(0, m_perFrameUniform, m_perFrameDataOffset, m_perFrameDataStride);
        RenderContext::BindUniformBuffer(1, mat.GetParamBuffer());

        const auto& textures = mat.GetTextures();
        const auto& samplers = mat.GetSamplers();

        for (u32 slot {0}; slot < textures.size(); ++slot)
        {
            if (textures[slot])
                RenderContext::BindTexture(slot, textures[slot]);
            
            if (samplers[slot])
                RenderContext::BindSampler(slot, samplers[slot]);
        }

        DrawMesh(mesh);
        m_perFrameDataOffset += m_perFrameDataStride;
    }

    void Renderer::SetCamera(Camera& cam)
    {
        m_cam = &cam;
    }

    void Renderer::OnWindowResize(WindowResizeEvent& event)
    {
        m_cam->Resize(event.GetWidth(), event.GetHeight());
        RenderContext::SetViewport(0, 0, event.GetWidth(), event.GetHeight());
    }

    Renderer::~Renderer()
    {
        delete m_perFrameUniform;
    }
}