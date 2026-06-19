#pragma once

#include "Types.hpp"
#include "Shader.hpp"

#include <unordered_map>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

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

    std::vector<MaterialParam> params;
};

struct ParamEntry {
    usize             offset;
    usize             size;
    MaterialParamType type;
};

class Material {
private:
    Buffer* m_parametersBuffer = nullptr;
    u32     m_textureCount     = 0;
    MaterialDesc m_desc;

    ShaderProgram* m_program   = nullptr;
    
    std::unordered_map<std::string, ParamEntry> m_paramMap;
    std::vector<Texture*> m_textures;
    std::vector<Sampler*> m_samplers;

    void Init();
    void UploadParam(const char* name, const void* data, usize size);

public:
    Material() = default;
    Material(const MaterialDesc& desc);
    ~Material();

    void SetDescription(const MaterialDesc& desc);

    void SetParamInt        (const char* name, s32 v);
    void SetParamFloat      (const char* name, f32 v);
    void SetParamVector2    (const char* name, const glm::vec2& v);
    void SetParamVector3    (const char* name, const glm::vec3& v);
    void SetParamVector4    (const char* name, const glm::vec4& v);
    void SetParamMatrix4    (const char* name, const glm::mat4& v);

    void SetParamTexture(const char* name, Texture* texture);
    void SetParamSampler(const char* name, Sampler* sampler);

    ShaderProgram*               GetProgram()     const { return m_program; }
    Shader*                      GetVS()          const { return m_desc.vs; }
    Shader*                      GetFS()          const { return m_desc.fs; }
    Buffer*                      GetParamBuffer() const { return m_parametersBuffer; }
    u32                          GetTexCount()    const { return m_textureCount; }
    const std::vector<Texture*>& GetTextures()    const { return m_textures; }
    const std::vector<Sampler*>& GetSamplers()    const { return m_samplers; }
};

} // namespace Acroy