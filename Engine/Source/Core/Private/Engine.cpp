#include "Types.hpp"
#include "Engine.hpp"
#include "RHI.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "GraphicsPipeline.hpp"
#include "FrameBuffer.hpp"
#include "Buffer.hpp"

#include <iostream>

namespace Acroy
{

    static const char* ReadFile(const char* filepath, usize& outSize)
    {
        FILE* file = fopen(filepath, "rb");
        if (!file)
        {
            std::cout << "Failed to open file: " << filepath << std::endl;
            outSize = 0;
            return nullptr;
        }

        fseek(file, 0, SEEK_END);
        outSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = new char[outSize];
        fread(buffer, 1, outSize, file);
        fclose(file);

        return buffer;
    }

    struct GlobalUniforms
    {
        float time;
    };

    Engine::Engine()
    {
        WindowProps props = {
            .width  = 1600,
            .height = 900,
            .fullscreen = false,
            .title  = "Acroy Engine",
        };

        _window = new Window(props);

        RHI::Init(_window);

        _window->SetVSync(true);

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


        const char* vertexPath = "Shaders/vertex.spv";
        const char* fragmentPath = "Shaders/fragment.spv";

        usize vertexSize = 0;
        const char* vertexBinary = ReadFile(vertexPath, vertexSize);
        if (!vertexBinary)        {
            std::cout << "Failed to read vertex shader file: " << vertexPath << std::endl;
            return;
        }

        usize fragmentSize = 0;
        const char* fragmentBinary = ReadFile(fragmentPath, fragmentSize);
        if (!fragmentBinary) {
            std::cout << "Failed to read fragment shader file: " << fragmentPath << std::endl;
            delete[] vertexBinary;
            return;
        }

        ShaderDesc vertexShaderDesc{};
        vertexShaderDesc.stage = ShaderStage::Vertex;
        vertexShaderDesc.entryPoint = "main";
        vertexShaderDesc.size = vertexSize;
        vertexShaderDesc.binary = vertexBinary;

        ShaderDesc fragmentShaderDesc{};
        fragmentShaderDesc.stage = ShaderStage::Fragment;
        fragmentShaderDesc.entryPoint = "main";
        fragmentShaderDesc.size = fragmentSize;
        fragmentShaderDesc.binary = fragmentBinary;

        Shader* vertexShader   = new Shader(vertexShaderDesc);
        Shader* fragmentShader = new Shader(fragmentShaderDesc);

        VertexAttribute vertexAttributes[3]{};
        vertexAttributes[0].location  = 0;
        vertexAttributes[0].componentCount = 3;
        vertexAttributes[0].stride    = sizeof(float) * 8;
        vertexAttributes[0].offset    = 0;
        vertexAttributes[0].type      = Type::Float;
        vertexAttributes[0].debugName = "aPosition";

        vertexAttributes[1].location  = 1;
        vertexAttributes[1].componentCount = 3;
        vertexAttributes[1].stride    = sizeof(float) * 8;
        vertexAttributes[1].offset    = sizeof(float) * 3;
        vertexAttributes[1].type      = Type::Float;
        vertexAttributes[1].debugName = "aColor";

        vertexAttributes[2].location  = 2;
        vertexAttributes[2].componentCount = 2;
        vertexAttributes[2].stride    = sizeof(float) * 8;
        vertexAttributes[2].offset    = sizeof(float) * 6;
        vertexAttributes[2].type      = Type::Float;
        vertexAttributes[2].debugName = "aTexCoord";

        GraphicsPipelineDesc pipelineDesc{};
        pipelineDesc.vertexShader        = vertexShader;
        pipelineDesc.fragmentShader      = fragmentShader;
        pipelineDesc.primitiveType       = PrimitiveType::TriangleList;
        pipelineDesc.numVertexAttributes = 3;
        pipelineDesc.vertexAttributes    = vertexAttributes;

        _pipeline = new GraphicsPipeline(pipelineDesc);

        delete vertexShader;
        delete fragmentShader;
        
        f32 vertices[] = {
            // positions         // colors           // texture coords
           -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
           -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f
        };

        u32 indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        BufferDesc vertexBufferDesc{};
        vertexBufferDesc.size = sizeof(vertices);
        vertexBufferDesc.usage = BufferUsage::Static;
        vertexBufferDesc.type = BufferType::Vertex;
        vertexBufferDesc.cpuWritable = false;
        vertexBufferDesc.persistentMap = false;

        _vertexBuffer = new Buffer(vertexBufferDesc);
        _vertexBuffer->UploadData(vertices, sizeof(vertices));


        BufferDesc indexBufferDesc{};
        indexBufferDesc.size = sizeof(indices);
        indexBufferDesc.type = BufferType::Index;
        indexBufferDesc.usage = BufferUsage::Static;
        indexBufferDesc.cpuWritable = false;
        indexBufferDesc.persistentMap = false;

        _indexBuffer = new Buffer(indexBufferDesc);
        _indexBuffer->UploadData(indices, sizeof(indices));
    }

    void Engine::Run()
    {
        while (!_window->ShouldClose())
        {
            const glm::ivec2 windowSize = _window->GetSize();
            _frameBuffer->Resize(windowSize);

            RHI::BeginRenderPass(_frameBuffer, RenderPassDesc{
                .clearColor = glm::vec4(1.0f)
            });
            
            RHI::BindGraphicsPipeline(_pipeline);
            RHI::BindVertexBuffer(0, _vertexBuffer, 0);
            RHI::BindIndexBuffer(_indexBuffer, IndexType::UInt32);

            RHI::DrawIndexed(6);

            RHI::EndRenderPass();
            RHI::Present(_frameBuffer);

            _window->Update();
        }
    }

    Engine::~Engine()
    {
        std::cout << "Shutting down engine..." << std::endl;

        delete _vertexBuffer;
        delete _indexBuffer;
        delete _pipeline;
        delete _frameBuffer;
        delete _window;
    }
}