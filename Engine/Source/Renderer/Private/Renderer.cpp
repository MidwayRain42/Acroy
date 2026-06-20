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
#include "Skybox.hpp"

#include <cassert>
#include <iostream>

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

        BufferDesc frameUBODesc {    
            .size = sizeof(PerFrameData),
            .type = BufferType::Uniform,
            .usage = BufferUsage::Dynamic,
            .cpuWritable = true,
            .persistentMap = true
        };
            
        m_frameUniformBuffer = new Buffer(frameUBODesc);

        const u64 uboAlignment = RenderContext::GetUniformBufferOffsetAlignment();
        m_objectStride = AlignUp(sizeof(PerObjectData), uboAlignment);
        const u64 objectBufferSize = m_objectStride * 128;

        BufferDesc objectUBODesc {
            .size = static_cast<u32>(objectBufferSize),
            .type = BufferType::Uniform,
            .usage = BufferUsage::Dynamic,
            .cpuWritable = true,
            .persistentMap = true
        };

        m_objectUniformBuffer = new Buffer(objectUBODesc);
    }

    void Renderer::BeginFrame()
    {
        m_renderState.depthWrite = true;
        m_renderState.depthFunc  = DepthFunc::Less;
        RenderContext::ApplyRenderState(m_renderState);
        RenderContext::Clear({});

        if (m_cam)
        {
            m_perFrameData.view = glm::inverse(m_cam->GetTransform().GetMatrix());
            m_perFrameData.proj = m_cam->GetProj();
        }
        
        m_perFrameData.time = m_window->GetTime();
        
        m_frameUniformBuffer->UploadData(&m_perFrameData, sizeof(PerFrameData));
        RenderContext::BindUniformBuffer(0, m_frameUniformBuffer);
        m_objectOffset = 0;
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

        m_perObjectData.model = transform.GetMatrix();

        m_objectUniformBuffer->UploadData(&m_perObjectData, sizeof(PerObjectData), m_objectOffset);

        RenderContext::BindUniformBuffer(1, m_objectUniformBuffer, m_objectOffset, m_objectStride);

        RenderContext::BindUniformBuffer(2, mat.GetParamBuffer());

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
        m_objectOffset += m_objectStride;
    }

    void Renderer::DrawSkybox(const Skybox& sky)
    {
        RenderContext::BindShaderProgram(sky.GetProgram());
        RenderContext::BindVertexArray(sky.GetVAO());
        RenderContext::BindTexture(0, sky.GetTexture());
        RenderContext::BindSampler(0, sky.GetSampler());


        m_renderState.depthWrite = false;
        m_renderState.depthFunc  = DepthFunc::Lequal;
        RenderContext::ApplyRenderState(m_renderState);
        RenderContext::Draw(36);
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
        delete m_frameUniformBuffer;
        delete m_objectUniformBuffer;
    }
}