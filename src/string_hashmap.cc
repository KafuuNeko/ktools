#include "basic.hpp"
#include "def_export.hpp"
#include <cinttypes>
#include <functional>
#include <string>
#include <unordered_map>


template <typename _Key, typename _Value>
using HashMapT = std::unordered_map<_Key, _Value>;
using StringMapT = HashMapT<std::string, std::string>;
using StringMapPtr = StringMapT*;

EXPORT_API(StringMapPtr, new_string_map, 0)
()
{
    return new StringMapT {};
}

EXPORT_API(void, delete_string_map, (parameterSizeof<string_map_ptr>()))
(StringMapPtr ptr)
{
    delete ptr;
}

EXPORT_API(void, put_string_map, (parameterSizeof<string_map_ptr, cstr_t, cstr_t>()))
(StringMapPtr ptr, cstr_t key, cstr_t value)
{
    (*ptr)[key] = value;
}

EXPORT_API(cstr_t, find_string_map, (parameterSizeof<string_map_ptr, cstr_t>()))
(StringMapPtr ptr, cstr_t key)
{
    auto it = ptr->find(key);
    return (it == ptr->end()) ? "" : it->second.c_str();
}

EXPORT_API(void, erase_string_map, (parameterSizeof<string_map_ptr, cstr_t>()))
(StringMapPtr ptr, cstr_t key)
{
    ptr->erase(key);
}
