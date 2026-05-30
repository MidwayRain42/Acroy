#include "Material.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <cassert>

namespace Acroy {

    static usize GetParamSize(const MaterialParam& p) {
        switch (p.type) {
            case MaterialParamType::Int:       return 4;
            case MaterialParamType::Float:     return 4;
            case MaterialParamType::Float2:    return 8;
            case MaterialParamType::Float3:    return 12;
            case MaterialParamType::Float4:    return 16;
            case MaterialParamType::Matrix4x4: return 64;
            case MaterialParamType::Texture:   return 0;
        }

        assert(false && "Unknown MaterialParamType");
        return 0;
    }

    Material::Material(const MaterialDesc& desc)
    {
        _vs = desc.vs;
        _fs = desc.fs;

        if (desc.paramCount > 0 && desc.params != nullptr)
        {
            usize runningOffset = 0;

            for (u32 i = 0; i < desc.paramCount; i++)
            {
                const MaterialParam& param = desc.params[i];
                const usize          size  = GetParamSize(param);

                if (param.type == MaterialParamType::Texture)
                {
                    _paramMap.emplace(param.name, ParamEntry{
                        .offset = _textureCount,
                        .size   = 0,
                        .type   = param.type
                    });
                    
                    _textureCount++;
                }
                else
                {
                    _paramMap.emplace(param.name, ParamEntry{
                        .offset = runningOffset,
                        .size   = size,
                        .type   = param.type
                    });

                    runningOffset += size;
                }
            }

            if (_textureCount > 0)
            {
                _textures.resize(_textureCount, nullptr);
                _samplers.resize(_textureCount, nullptr);
            }

            if (runningOffset > 0)
            {
                BufferDesc d{};
                d.cpuWritable   = true;
                d.persistentMap = true;
                d.size          = runningOffset;
                d.type          = BufferType::Uniform;
                d.usage         = BufferUsage::Dynamic;

                _parametersBuffer = new Buffer(d);
            }
        }
    }

    Material::~Material()
    {
        delete _parametersBuffer;
        delete _vs;
        delete _fs;
    }

    void Material::UploadParam(const char* name, const void* data, usize size)
    {
        assert(_parametersBuffer != nullptr && "No parameter buffer on this material");

        auto it = _paramMap.find(name);
        assert(it != _paramMap.end() && "Unknown material parameter name");

        const ParamEntry& entry = it->second;
        assert(size <= entry.size && "Upload size exceeds parameter size");

        _parametersBuffer->UploadData(data, size, entry.offset);
    }

    void Material::SetParamInt(const char* name, s32 v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Int);
        UploadParam(name, &v, sizeof(s32));
    }

    void Material::SetParamFloat(const char* name, f32 v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Float);
        UploadParam(name, &v, sizeof(f32));
    }

    void Material::SetParamVector2(const char* name, const glm::vec2& v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Float2);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 2);
    }

    void Material::SetParamVector3(const char* name, const glm::vec3& v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Float3);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 3);
    }

    void Material::SetParamVector4(const char* name, const glm::vec4& v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Float4);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 4);
    }

    void Material::SetParamMatrix4(const char* name, const glm::mat4& v)
    {
        assert(_paramMap.count(name) && _paramMap.at(name).type == MaterialParamType::Matrix4x4);
        UploadParam(name, glm::value_ptr(v), sizeof(f32) * 16);
    }


    void Material::SetParamTexture(const char* name, Texture* texture)
    {
        auto it = _paramMap.find(name);
        assert(it != _paramMap.end()                         && "Unknown material parameter name");
        assert(it->second.type == MaterialParamType::Texture && "Parameter is not a texture");

        const u32 slot = static_cast<u32>(it->second.offset);
        assert(slot < _textureCount);

        _textures[slot] = texture;
    }

    void Material::SetParamSampler(const char* name, Sampler* sampler)
    {
        auto it = _paramMap.find(name);
        assert(it != _paramMap.end()                         && "Unknown material parameter name");
        assert(it->second.type == MaterialParamType::Texture && "Parameter is not a texture");

        const u32 slot = static_cast<u32>(it->second.offset);
        assert(slot < _textureCount);

        _samplers[slot] = sampler;
    }

} // namespace Acroy