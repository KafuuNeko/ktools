#pragma once

#include <algorithm>
#include <string_view>

using cstr_t = const char*;

inline cstr_t newCstr(std::string_view str) {

    auto temp = new char[str.length() + 1];
    std::copy(str.cbegin(), str.cend(), temp);
    temp[str.length()] = '\0';

    return temp;
}
