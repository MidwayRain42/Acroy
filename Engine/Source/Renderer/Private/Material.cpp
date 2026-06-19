#include "Material.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <cassert>

namespace Acroy {

    static usize AlignUp(usize value, usize alignment)
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    static usize GetStd140Alignment(MaterialParamType type)
    {
        switch (type)
        {
            case MaterialParamType::Int:
            case MaterialParamType::Float:
                return 4;

            case MaterialParamType::Float2:
                return 8;

            case MaterialParamType::Float3:
            case MaterialParamType::Float4:
            case MaterialParamType::Matrix4x4:
                return 16;

            case MaterialParamType::Texture:
                return 0;
        }

        assert(false && "Unknown MaterialParamType");
        return 0;
    }

    static usize GetParamSize(const MaterialParam& p)
    {
        switch (p.type)
        {
            case MaterialParamType::Int:       return 4;
            case MaterialParamType::Float:     return 4;
            case MaterialParamType::Float2:    return 8;

            // std140 stores vec3 in 16 bytes
            case MaterialParamType::Float3:    return 16;

            case MaterialParamType::Float4:    return 16;
            case MaterialParamType::Matrix4x4: return 64;

            case MaterialParamType::Texture:   return 0;
        }

        assert(false && "Unknown MaterialParamType");
        return 0;
    }

    Material::Material(const MaterialDesc& desc)
    {
        m_desc = desc;
        Init();
    }


    void Material::SetDescription(const MaterialDesc& desc)
    {
        m_desc = desc;
        Init();
    }

    void Material::Init()
    {
        m_paramMap.clear();

        m_textureCount = 0;

        usize runningOffset = 0;

        for (const auto& param : m_desc.params)
        {
            if (param.type == MaterialParamType::Texture)
            {
                m_paramMap.emplace(param.name, ParamEntry{
                    .offset = m_textureCount,
                    .size   = 0,
                    .type   = param.type
                });

                ++m_textureCount;
                continue;
            }

            const usize alignment = GetStd140Alignment(param.type);
            const usize size      = GetParamSize(param);

            runningOffset = AlignUp(runningOffset, alignment);

            m_paramMap.emplace(param.name, ParamEntry{
                .offset = runningOffset,
                .size   = size,
                .type   = param.type
            });

            runningOffset += size;
        }

        // std140 struct alignment
        runningOffset = AlignUp(runningOffset, 16);

        if (m_textureCount > 0)
        {
            m_textures.resize(m_textureCount, nullptr);
            m_samplers.resize(m_textureCount, nullptr);
        }

        if (runningOffset > 0)
        {
            BufferDesc d{};
            d.cpuWritable   = true;
            d.persistentMap = true;
            d.size          = runningOffset;
            d.type          = BufferType::Uniform;
            d.usage         = BufferUsage::Dynamic;

            m_parametersBuffer = new Buffer(d);
        }

        m_program = new ShaderProgram({ m_desc.vs, m_desc.fs });
    }

    Material::~Material()
    {
        delete m_program;
        delete m_parametersBuffer;
    }

    void Material::UploadParam(const char* name, const void* data, usize size)
    {
        assert(m_parametersBuffer != nullptr && "No parameter buffer on this material");

        auto it = m_paramMap.find(name);
        assert(it != m_paramMap.end() && "Unknown material parameter name");

        const ParamEntry& entry = it->second;
        assert(size <= entry.size && "Upload size exceeds parameter size");

        m_parametersBuffer->UploadData(data, size, entry.offset);
    }

    void Material::SetParamInt(const char* name, s32 v)
    {
        assert(m_paramMap.count(name) && m_paramMap.at(name).type == MaterialParamType::Int);
        UploadParam(name, &v, sizeof(s32));
    }

    void Material::SetParamFloat(const char* name, f32 v)
    {
        assert(m_paramMap.count(name) && m_paramMap.at(name).type == MaterialParamType::Float);
        UploadParam(name, &v, sizeof(f32));
    }

    void Material::SetParamVector2(const char* name, const glm::vec2& v)
    {
        assert(m_paramMap.count(name) && m_paramMap.at(name).type == MaterialParamType::Float2);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 2);
    }

    void Material::SetParamVector3(const char* name, const glm::vec3& v)
    {
        assert(m_paramMap.count(name) &&
            m_paramMap.at(name).type == MaterialParamType::Float3);

        const glm::vec4 padded(v, 0.0f);

        UploadParam(name, glm::value_ptr(padded), sizeof(glm::vec4));
    }

    void Material::SetParamVector4(const char* name, const glm::vec4& v)
    {
        assert(m_paramMap.count(name) && m_paramMap.at(name).type == MaterialParamType::Float4);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 4);
    }

    void Material::SetParamMatrix4(const char* name, const glm::mat4& v)
    {
        assert(m_paramMap.count(name) && m_paramMap.at(name).type == MaterialParamType::Matrix4x4);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 16);
    }


    void Material::SetParamTexture(const char* name, Texture* texture)
    {
        auto it = m_paramMap.find(name);
        assert(it != m_paramMap.end() && "Unknown material parameter name");
        assert(it->second.type == MaterialParamType::Texture && "Parameter is not a texture");

        const u32 slot = static_cast<u32>(it->second.offset);
        assert(slot < m_textureCount);

        m_textures[slot] = texture;
    }

    void Material::SetParamSampler(const char* name, Sampler* sampler)
    {
        auto it = m_paramMap.find(name);
        assert(it != m_paramMap.end()                         && "Unknown material parameter name");
        assert(it->second.type == MaterialParamType::Texture && "Parameter is not a texture");

        const u32 slot = static_cast<u32>(it->second.offset);
        assert(slot < m_textureCount);

        m_samplers[slot] = sampler;
    }

} // namespace Acroy