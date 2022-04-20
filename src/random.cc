#include "def_export.hpp"
#include <cinttypes>
#include <random>
#include <stdint.h>

EXPORT_API(uint32_t, random, 0)()
{
    return std::random_device()();
}

EXPORT_API(std::mt19937_64*, new_random_twister_engine, (parameterSizeof<uint32_t>()))(uint32_t seed)
{
    return new std::mt19937_64(seed);
}

EXPORT_API(void, free_random_twister_engine, (parameterSizeof<std::mt19937_64*>()))(std::mt19937_64* mt)
{
    if (mt) {
        delete mt;
    }
}

EXPORT_API(int64_t, random_integer, (parameterSizeof<std::mt19937_64* mt, int64_t, int64_t>()))
(std::mt19937_64* mt, int64_t left, int64_t right)
{
    //[left, right]
    return std::uniform_int_distribution<int64_t>(left, right)(*mt);
}

EXPORT_API(double, random_real, (parameterSizeof<std::mt19937_64* mt, double, double>()))
(std::mt19937_64* mt, double left, double right)
{
    //[left, right)
    return std::uniform_real_distribution<double>(left, right)(*mt);
}





