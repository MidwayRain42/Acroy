#pragma once

#include "Types.hpp"

#include <string>
#include <vector>

namespace Acroy
{
    std::string LoadFileText(const std::string& path);
    std::vector<u8> LoadFileBinary(const std::string& path);
}