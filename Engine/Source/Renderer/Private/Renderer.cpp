#include "Renderer.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "RHI.hpp"
#include "GraphicsPipeline.hpp"

#include "Mesh.hpp"
#include "Material.hpp"

#include <iostream>
#include <unordered_map>

namespace Acroy
{
    u64 HashPipelineKey(const PipelineKey& k)
    {
        u64 h = 0;

        auto mix = [&](u64 v)
        {
            h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        };

        mix((u64)k.vs);
        mix((u64)k.fs);
        
        for (const auto& a : k.vertexAttributes)
        {
            mix(a.location);
            mix(a.offset);
            mix(a.stride);
        }

        mix((u64)k.primitive);

        return h;
    }

    GraphicsPipeline* PipelineCache::GetOrCreate(const PipelineKey& pipelineKey)
    {
        u64 key = HashPipelineKey(pipelineKey);

        auto it = _pipelines.find(key);
        if (it != _pipelines.end())
            return it->second;

        GraphicsPipelineDesc desc{};
        desc.vertexShader        = pipelineKey.vs;
        desc.fragmentShader      = pipelineKey.fs;
        desc.primitiveType       = pipelineKey.primitive;
        desc.vertexAttributes    = pipelineKey.vertexAttributes;

        GraphicsPipeline* pipeline = new GraphicsPipeline(desc);
        _pipelines.emplace(key, pipeline);
        return pipeline;
    }

    PipelineCache::~PipelineCache()
    {
        for (auto& [key, pipeline] : _pipelines)
            delete pipeline;
    }

    Renderer::Renderer(const RendererDesc& desc) : _window(desc.window)
    {
        std::cout << "Initializing Renderer" << std::endl;

        RHI::Init(desc.window);
        desc.window->SetVSync(desc.enableVSync);

        glm::ivec2 windowSize = _window->GetSize();

        TextureDesc swapChainTexDesc{};
        swapChainTexDesc.width = static_cast<u32>(windowSize.x);
        swapChainTexDesc.height = static_cast<u32>(windowSize.y);
        swapChainTexDesc.type = TextureType::Texture2D;
        swapChainTexDesc.format = Format::RGBA8;
        swapChainTexDesc.mipLevels = 1;

        Texture* swapChainTex = new Texture(swapChainTexDesc);


        TextureDesc depthTextureDesc{};
        depthTextureDesc.width = static_cast<u32>(windowSize.x);
        depthTextureDesc.height = static_cast<u32>(windowSize.y);
        depthTextureDesc.mipLevels = 1;
        depthTextureDesc.format = Format::DEPTH32F_STENCIL8;
        depthTextureDesc.type = TextureType::Texture2D;

        Texture* depthTexture = new Texture(depthTextureDesc);


        FrameBufferDesc fbDesc{};
        fbDesc.colorAttachments.push_back(swapChainTex);
        fbDesc.depthAttachment = depthTexture;

        _frameBuffer = new FrameBuffer(fbDesc);
    }

    void Renderer::BeginFrame()
    {
        const glm::ivec2 windowSize = _window->GetSize();
        _frameBuffer->Resize(windowSize);

        RHI::BeginRenderPass(_frameBuffer, {
            .clearColor = glm::vec4(0.0f)
        });
    }

    void Renderer::EndFrame()
    {
        RHI::EndRenderPass();
        RHI::Present(_frameBuffer);
    }

    void Renderer::DrawMesh(Mesh* mesh)
    {
        RHI::BindVertexBuffer(0, mesh->GetVertexBuffer());
        RHI::BindIndexBuffer(mesh->GetIndexBuffer());
        RHI::DrawIndexed(mesh->GetIndexCount());
    }

    void Renderer::DrawMesh(Mesh* mesh, Material* mat)
    {
        PipelineKey key{};
        key.fs = mat->GetFS();
        key.vs = mat->GetVS();
        key.primitive = PrimitiveType::TriangleList;
        key.vertexAttributes = mesh->GetVertexAttributes();

        GraphicsPipeline* pipeline = _pipelineCache.GetOrCreate(key);

        RHI::BindGraphicsPipeline(pipeline);

        RHI::BindUniformBuffer(0, mat->GetParamBuffer(), 0);

        const auto& textures = mat->GetTextures();
        const auto& samplers = mat->GetSamplers();

        for (u32 slot = 0; slot < (u32)textures.size(); ++slot)
        {
            if (textures[slot])
                RHI::BindTexture(slot, textures.at(slot));
            
            if (samplers[slot])
                RHI::BindSampler(slot, samplers.at(slot));
        }

        DrawMesh(mesh);
    }


    Renderer::~Renderer()
    {
        delete _frameBuffer;
    }
}