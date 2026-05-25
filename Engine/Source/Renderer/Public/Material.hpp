#pragma once

#include "Types.hpp"
#include <unordered_map>
#include <string>
#include <vector>

namespace Acroy {

class GraphicsPipeline;
class Shader;
class Buffer;
class Texture;
class Sampler;

struct VertexAttribute;

enum class MaterialParamType {
    Int,
    Float,
    Float2,
    Float3,
    Float4,
    Matrix4x4,
    Texture
};

struct MaterialParam {
    const char*       name;
    MaterialParamType type;
};

struct MaterialDesc {
    Shader* vs;
    Shader* fs;

    MaterialParam* params;
    u32            paramCount;
};

struct ParamEntry {
    usize             offset;
    usize             size;
    MaterialParamType type;
};

class Material {
private:
    Buffer* _parametersBuffer = nullptr;
    Shader* _vs               = nullptr;
    Shader* _fs               = nullptr;
    u32     _textureCount     = 0;

    std::unordered_map<std::string, ParamEntry> _paramMap;
    std::vector<Texture*> _textures;
    std::vector<Sampler*> _samplers;

    void UploadParam(const char* name, const void* data, usize size);

public:
    Material(const MaterialDesc& desc);
    ~Material();

    void SetParamInt       (const char* name, s32 v);
    void SetParamFloat     (const char* name, f32 v);
    void SetParamFloat2    (const char* name, f32 x, f32 y);
    void SetParamFloat3    (const char* name, f32 x, f32 y, f32 z);
    void SetParamFloat4    (const char* name, f32 x, f32 y, f32 z, f32 w);
    void SetParamMatrix4x4 (const char* name, const f32* colMajor16);

    void SetParamTexture(const char* name, Texture* texture);
    void SetParamSampler(const char* name, Sampler* sampler);

    inline Shader*                      GetVS()          const { return _vs; }
    inline Shader*                      GetFS()          const { return _fs; }
    inline Buffer*                      GetParamBuffer() const { return _parametersBuffer; }
    inline u32                          GetTexCount()    const { return _textureCount; }
    inline const std::vector<Texture*>& GetTextures()    const { return _textures; }
    inline const std::vector<Sampler*>& GetSamplers()    const { return _samplers; }
};

} // namespace Acroy