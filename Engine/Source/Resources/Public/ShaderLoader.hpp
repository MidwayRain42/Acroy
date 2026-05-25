#pragma once

namespace Acroy {

    class Shader;
    enum class ShaderStage;

    class ShaderLoader {
    public:
        static Shader* FromFile(const char* path, ShaderStage stage);
    };

}
