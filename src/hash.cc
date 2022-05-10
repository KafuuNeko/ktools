#include "basic.hpp"
#include "def_export.hpp"
#include "hash.h"


EXPORT_API(int32_t, string_hash_i32, (parameterSizeof<cstr_t>()))
(cstr_t str)
{
    return stringHashI32(str);
}
