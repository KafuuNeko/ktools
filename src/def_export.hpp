#pragma once
#include <cinttypes>

#if defined(_MSC_VER)

 #define EXPORT_API(ReturnType, FuncName, ParamsSize)                                \
     __pragma(comment(linker, "/EXPORT:" #FuncName "=_" #FuncName "@" #ParamsSize)) \
     extern "C" __declspec(dllexport) ReturnType __stdcall FuncName

#else

#define EXPORT_API(ReturnType, FuncName, ParamsSize) \
    extern "C" __declspec(dllexport) ReturnType __stdcall FuncName

#endif

template<class T, class... Args>
constexpr size_t parameterSizeof() {
    if constexpr (sizeof...(Args) == 0) {
        return sizeof(T);
    } else {
        return parameterSizeof<Args...>() + sizeof(T);
    }
}


