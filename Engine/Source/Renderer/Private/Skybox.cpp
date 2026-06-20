#include "Skybox.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"


#include <stb_image.h>
#include <iostream>

namespace Acroy
{
    Skybox::Skybox(std::array<std::string_view, 6> images)
    {
        TextureDesc td;

        td.mipLevels = 1;
        td.type = TextureType::CubeMap;

        std::array<unsigned char*, 6> imageData;

        stbi_set_flip_vertically_on_load(false);
        
        for (u32 i{0}; i < 6; i++)
        {
            int x, y, c;
            imageData[i] = stbi_load(images[i].data(), &x, &y, &c, 0);
            
            if (!imageData[i])
            {
                std::cout << "Failed to load cubemap iamge: " + std::string(images[i]) << std::endl;
                assert(false);
            }

            // all cubemap images have to be the same size
            if (i == 0)
            {
                td.width = x;
                td.height = y;
            }
            else
            {
                assert(x == td.width);
                assert(y == td.height);
            }

            Format format;

            switch (c)
            {
                case 1: format = Format::RED;   break;
                case 3: format = Format::RGB8;  break;
                case 4: format = Format::RGBA8; break;

                default: assert(false && "Unknown Texture Type");
            }

            if (i == 0)
            {
                td.format = format;
            }
            else
            {
                assert(format == td.format);
            }
        }
        
        m_texture = new Texture(td);

        for (u32 i{0}; i < 6; i++)
        {
            m_texture->UploadData(imageData[i], 0, 0, i);
            stbi_image_free(imageData[i]);
        }

        SamplerDesc sd {
            .minFilter = SamplerFilter::Linear,
            .magFilter = SamplerFilter::Linear,
            .wrapS     = SamplerWrap::ClampToEdge,
            .wrapT     = SamplerWrap::ClampToEdge,
            .wrapR     = SamplerWrap::ClampToEdge,
        };

        m_sampler = new Sampler(sd);


        float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
 
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        m_skyboxVA = new VertexArray();

        m_vbo = new Buffer(BufferDesc {
            .size = sizeof(skyboxVertices),
            .type = BufferType::Vertex,
            .usage = BufferUsage::Static,
            .cpuWritable = false,
            .persistentMap = false
        });

        m_vbo->UploadData(skyboxVertices, sizeof(skyboxVertices));

        VertexAttribute attrib {
            .location = 0,
            .componentCount = 3,
            .stride = sizeof(f32) * 3
        };

        m_skyboxVA->AddVertexBuffer(m_vbo, &attrib, 1, 0, 0, attrib.stride);

        auto vs = ResourceManager::LoadShader("Bin/Shaders/Skybox.vert.spv", ShaderStage::Vertex);
        auto fs = ResourceManager::LoadShader("Bin/Shaders/Skybox.frag.spv", ShaderStage::Fragment);

        m_program = new ShaderProgram({ vs.get(), fs.get() });
    }

    Skybox::~Skybox()
    {
        delete m_vbo;
        delete m_texture;
        delete m_sampler;
        delete m_skyboxVA;
    }
}
