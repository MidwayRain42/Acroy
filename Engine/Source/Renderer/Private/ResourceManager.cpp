#include "ResourceManager.hpp"
#include <fstream>
#include <cmath>
#include <iostream>
#include <stb_image.h>
#include <tiny_obj_loader.h>

namespace Acroy
{
    std::shared_ptr<Texture> ResourceManager::LoadTexture(std::string_view path)
    {
        stbi_set_flip_vertically_on_load(true);

        TextureDesc desc{};

        desc.type = TextureType::Texture2D;

        s32 x, y, channels;
        u8* bytes = stbi_load(path.data(), &x, &y, &channels, 0);

        if (!bytes)
        {
            printf("Failed to load image: %s\n", stbi_failure_reason());
            assert(false);
        }

        Format format;

        switch (channels)  
        {
            case 1: format = Format::RED;   break;
            case 3: format = Format::RGB8;  break;
            case 4: format = Format::RGBA8; break;

            default: assert(false && "Unknown Texture Type");
        }

        desc.format = format;
        desc.width  = x;
        desc.height = y;
        desc.mipLevels = 1 + static_cast<s32>(std::floor(std::log2(static_cast<f32>(std::max(x, y)))));

        usize size = x * y * channels * sizeof(u8);

        std::shared_ptr<Texture> tex = std::make_shared<Texture>(desc);
        tex->UploadData(bytes, size);

        stbi_image_free(bytes);

        return tex;
    }

    std::shared_ptr<Mesh> ResourceManager::LoadOBJ(std::string_view path)
    {
        tinyobj::ObjReaderConfig readerConfig;

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(path.data(), readerConfig)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            assert(false);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        const auto& attrib  = reader.GetAttrib();
        const auto& shapes  = reader.GetShapes();

        struct Vertex {
            float px, py, pz;
            float nx, ny, nz;
            float u, v;
        };

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        for (const auto& shape : shapes)
        {
            size_t indexOffset = 0;

            for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
            {
                size_t fv = shape.mesh.num_face_vertices[f];

                for (size_t v = 0; v < fv; ++v)
                {
                    const tinyobj::index_t& idx = shape.mesh.indices[indexOffset + v];

                    Vertex vert{};

                    // Position
                    vert.px = attrib.vertices[3 * idx.vertex_index + 0];
                    vert.py = attrib.vertices[3 * idx.vertex_index + 1];
                    vert.pz = attrib.vertices[3 * idx.vertex_index + 2];

                    // Normal
                    if (idx.normal_index >= 0)
                    {
                        vert.nx = attrib.normals[3 * idx.normal_index + 0];
                        vert.ny = attrib.normals[3 * idx.normal_index + 1];
                        vert.nz = attrib.normals[3 * idx.normal_index + 2];
                    }

                    // UV
                    if (idx.texcoord_index >= 0)
                    {
                        vert.u = attrib.texcoords[2 * idx.texcoord_index + 0];
                        vert.v = attrib.texcoords[2 * idx.texcoord_index + 1];
                    }

                    indices.push_back(vertices.size());
                    vertices.push_back(vert);
                }

                indexOffset += fv;
            }
        }

        MeshData data{};
        data.indexCount   = indices.size();
        data.verticesSize = sizeof(Vertex) * vertices.size();
        data.vertices     = &vertices.data()->px;
        data.indices      = indices.data();
        data.layout       = VertexLayout::PosNormalUV();

        return std::make_shared<Mesh>(data);
    }

    std::shared_ptr<Shader> ResourceManager::LoadShader(std::string_view path, ShaderStage stage)
    {
        std::vector<u8> shaderBinary = LoadFileBinary(path.data());

        ShaderDesc desc{};
        desc.stage      = stage;
        desc.entryPoint = "main";
        desc.size       = shaderBinary.size();
        desc.binary     = shaderBinary.data();

        return std::make_shared<Shader>(desc);
    }


    std::string ResourceManager::LoadFileText(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::in);

        if (!file)
            assert(false && "Failed to open file");

        std::string content;

        file.seekg(0, std::ios::end);
        content.resize(static_cast<usize>(file.tellg()));
        file.seekg(0, std::ios::beg);

        file.read(content.data(), content.size());

        return content;
    }

    std::vector<u8> ResourceManager::LoadFileBinary(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary);

        if (!file)
            assert(false && "Failed to open file");

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::vector<u8> buffer(size);

        file.read(reinterpret_cast<char*>(buffer.data()), size);

        return buffer;
    }
}