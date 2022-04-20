#include "def_export.hpp"

#include "basic.hpp"

EXPORT_API(cstr_t, new_string, (parameterSizeof<cstr_t>()))
(cstr_t str)
{
    return newCstr(str);
}

EXPORT_API(void, free_string, (parameterSizeof<cstr_t>()))
(cstr_t ptr)
{
    if (ptr) {
        delete[] ptr;
    }
}

