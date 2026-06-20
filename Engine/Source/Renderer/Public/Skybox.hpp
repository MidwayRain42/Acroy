#pragma once

#include "Texture.hpp"
#include <string_view>
#include <array>

namespace Acroy
{
    class VertexArray;
    class ShaderProgram;
    class Buffer;

    class Skybox
    {
    public:
        Skybox(std::array<std::string_view, 6> images);
        ~Skybox();

        Texture* GetTexture() const { return m_texture; }
        Sampler* GetSampler() const { return m_sampler; }
        VertexArray* GetVAO() const { return m_skyboxVA; }
        ShaderProgram* GetProgram() const { return m_program; }

    private:
        Texture* m_texture = nullptr;
        Sampler* m_sampler = nullptr;
        VertexArray* m_skyboxVA = nullptr;
        ShaderProgram* m_program = nullptr;
        Buffer* m_vbo;
    };
}