#include "FileUtils.hpp"

#include <fstream>
#include <stdexcept>

namespace Acroy
{

    std::string LoadFileText(const std::string& path)
    {
        std::ifstream file(path, std::ios::in);

        if (!file)
            throw std::runtime_error("Failed to open file: " + path);

        std::string content;

        file.seekg(0, std::ios::end);
        content.resize(static_cast<usize>(file.tellg()));
        file.seekg(0, std::ios::beg);

        file.read(content.data(), content.size());

        return content;
    }

    std::vector<u8> LoadFileBinary(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file)
            throw std::runtime_error("Failed to open file: " + path);

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        std::vector<u8> buffer(size);

        file.read(reinterpret_cast<char*>(buffer.data()), size);

        return buffer;
    }

} // namespace Acroy