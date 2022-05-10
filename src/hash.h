#pragma once

#include <string_view>

inline int32_t stringHashI32 (std::string_view str)
{
    /*
    uint32_t hashValue = 0;
    for (auto c : str) {
        hashValue = hashValue * 31 + c;
    }
    return static_cast<int32_t>(hashValue);
    */
    auto hashValue = std::hash<std::string_view>()(str) % (static_cast<uint32_t>(-1) >> 1);
    return static_cast<int32_t>(hashValue);
}

