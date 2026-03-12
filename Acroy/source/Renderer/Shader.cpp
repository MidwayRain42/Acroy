#include "AcroyPCH.hpp"
#include "Shader.hpp"
#include "Core/Log.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Acroy
{
    static constexpr uint32_t ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")   return GL_VERTEX_SHADER;
        if (type == "fragment") return GL_FRAGMENT_SHADER;

        ACROY_CORE_ASSERT(false, "Unknown shader type: {0}", type);
        return 0;
    }

    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    {
        std::unordered_map<uint32_t, std::string> sources;
        sources[GL_VERTEX_SHADER]   = vertexSource;
        sources[GL_FRAGMENT_SHADER] = fragmentSource;

        Compile(sources);
    }

    Shader::Shader(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            ACROY_CORE_ERROR("Could not open file '{0}'", filepath);
            return;
        }

        // fix: was passing filepath instead of result (the actual file contents)
        std::unordered_map<uint32_t, std::string> sources = PreProcess(result);
        Compile(sources);
    }

    std::unordered_map<uint32_t, std::string> Shader::PreProcess(const std::string& source)
    {
        std::unordered_map<uint32_t, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken); // = 5

        // start looking for #type from the beginning
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            // we found a "#type" but we need to make sure its actually
            // at the start of the line and not inside a comment or something
            // so lets find where the current line started
            size_t lineStart = source.rfind('\n', pos);
            lineStart = (lineStart == std::string::npos) ? 0 : lineStart + 1;

            // now check everything between the line start and our "#type"
            // if theres anything thats not a space or tab, skip this one
            for (size_t i = lineStart; i < pos; i++)
            {
                if (!isspace((unsigned char)source[i]))
                {
                    // something like "// #type vertex" would hit this
                    // just move on and search for the next #type
                    pos = source.find(typeToken, pos + typeTokenLength);
                    goto next_search;
                }
            }

            {
                // find the end of this line so we can grab the type name
                size_t eol = source.find_first_of("\r\n", pos);
                ACROY_CORE_ASSERT(eol != std::string::npos, "Syntax error: missing newline after #type");

                // skip past "#type" then eat any spaces before the actual name
                // handles stuff like "#type    fragment" with lots of spaces
                size_t typeBegin = pos + typeTokenLength;
                while (typeBegin < eol && isspace((unsigned char)source[typeBegin]))
                    typeBegin++;

                // also trim any trailing spaces or \r off the end
                // windows line endings would leave a \r otherwise
                size_t typeEnd = eol;
                while (typeEnd > typeBegin && isspace((unsigned char)source[typeEnd - 1]))
                    typeEnd--;

                // this should now be a clean string like "vertex" or "fragment"
                std::string type = source.substr(typeBegin, typeEnd - typeBegin);
                ACROY_CORE_ASSERT(!type.empty(), "Syntax error: empty shader type after #type");
                ACROY_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified: " + type);

                // skip the newline(s) after "#type vertex" to get to the actual glsl code
                size_t nextLinePos = source.find_first_not_of("\r\n", eol);
                ACROY_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error: no shader source after #type");

                // look ahead for the next #type so we know where this shader ends
                pos = source.find(typeToken, nextLinePos);

                // if there's no next #type, just take everything until end of file
                // otherwise take everything up to where the next #type starts
                shaderSources[ShaderTypeFromString(type)] =
                    (pos == std::string::npos)
                    ? source.substr(nextLinePos)
                    : source.substr(nextLinePos, pos - nextLinePos);
            }

            next_search:;
        }

        // at this point shaderSources should have 2 entries (or however many #type blocks there were)
        // each one mapped to its GL enum like GL_VERTEX_SHADER -> "glsl code here..."
        return shaderSources;
    }

    void Shader::Compile(const std::unordered_map<uint32_t, std::string>& sources)
    {
        m_rendererId = glCreateProgram();
        
        std::array<uint32_t, 2> shaderIDs;
        int shaderIDIndex = 0;

        for (auto& kv : sources)
        {
            uint32_t shaderType = kv.first;
            const char* source = kv.second.c_str();

            uint32_t shader = glCreateShader(shaderType);
            glShaderSource(shader, 1, &source, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                ACROY_CORE_ERROR(infoLog.data());
                ACROY_CORE_ASSERT(false, "Failed to compile shader");
                break;
            }

            glAttachShader(m_rendererId, shader);
            shaderIDs[shaderIDIndex++] = (shader);
        }

        glLinkProgram(m_rendererId);

        GLint isLinked = 0;
        glGetProgramiv(m_rendererId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_rendererId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_rendererId, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(m_rendererId);
            for (auto shader : shaderIDs)
                glDeleteShader(shader);

            ACROY_CORE_ERROR("Failed to link shader program");
            ACROY_CORE_ERROR(infoLog.data());

            return;
        }

        for (auto shader : shaderIDs)
            glDetachShader(m_rendererId, shader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_rendererId);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_rendererId);
    }

    void Shader::UnBind() const
    {
        glUseProgram(0);
    }

    void Shader::SetUniformInt(const char* name, const int value) const
    {
        glUniform1i(glGetUniformLocation(m_rendererId, name), value);
    }

    void Shader::SetUniformFloat(const char* name, const float value) const
    {
        glUniform1f(glGetUniformLocation(m_rendererId, name), value);
    }

    void Shader::SetUniformFloat2(const char* name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_rendererId, name), 1, glm::value_ptr(value));
    }

    void Shader::SetUniformFloat3(const char* name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_rendererId, name), 1, glm::value_ptr(value));
    }

    void Shader::SetUniformFloat4(const char* name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_rendererId, name), 1, glm::value_ptr(value));
    }

    void Shader::SetUniformMat3(const char* name, const glm::mat3& matrix) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_rendererId, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void Shader::SetUniformMat4(const char* name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_rendererId, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}