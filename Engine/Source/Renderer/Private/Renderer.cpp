#include "Renderer.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "FrameBuffer.hpp"
#include "RHI.hpp"

#include <iostream>

namespace Acroy
{
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
            .clearColor = { 0.8f, 0.1f, 0.3f, 1.0f }
        });
    }

    void Renderer::EndFrame()
    {
        RHI::EndRenderPass();
        RHI::Present(_frameBuffer);
    }

    Renderer::~Renderer()
    {
        delete _frameBuffer;
    }
}