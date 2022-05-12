#include "nlohmann/json.hpp"
#include "basic.hpp"
#include "def_export.hpp"
#include <corecrt.h>
#include <iterator>
#include <limits>
#include <sstream>
#include <stdint.h>
#include <string>

using JsonPtr = nlohmann::json*;

static const char* kLastError = "null";

struct StringVector {
    int32_t size;
    cstr_t* vec;
};

using StringVectorPtr = StringVector*;

EXPORT_API(void, json_free_vector, (parameterSizeof<StringVectorPtr>()))
(StringVectorPtr ptr)
{
    if (ptr) {
        if (ptr->vec) {
            for (size_t i = 0; i < ptr->size; ++i) {
                if (ptr->vec[i]) {
                    delete[] ptr->vec[i];
                }
            }
            delete[] ptr->vec;
        }
        ptr->vec = nullptr;
        ptr->size = 0;
    }
}

EXPORT_API(JsonPtr, json_new, 0)
()
{
    return new nlohmann::json;
}

EXPORT_API(JsonPtr, json_parse, (parameterSizeof<cstr_t>()))
(cstr_t text)
{
    try {
        return new nlohmann::json(nlohmann::json::parse(text));
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return nullptr;
    }
}

EXPORT_API(void, json_free, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    if (json) {
        delete json;
    }
}

EXPORT_API(bool, json_is_number, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_number() : false;
}

EXPORT_API(bool, json_is_number_integer, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_number_integer() : false;
}

EXPORT_API(bool, json_is_number_float, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_number_float() : false;
}

EXPORT_API(bool, json_is_null, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_null() : false;
}

EXPORT_API(bool, json_is_boolean, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_boolean() : false;
}

EXPORT_API(bool, json_is_string, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_string() : false;
}

EXPORT_API(bool, json_is_array, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_array() : false;
}

EXPORT_API(bool, json_is_object, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    return json ? json->is_object() : false;
}

EXPORT_API(cstr_t, json_text, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    if (!json) {
        return nullptr;
    }

    try {
        return newCstr((std::stringstream() << (*json)).str());
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return nullptr;
    }
}

EXPORT_API(bool, json_get, (parameterSizeof<json_ptr, cstr_t, json_ptr*>()))
(JsonPtr json, cstr_t key, JsonPtr* result)
{
    if (!json) {
        return false;
    }

    try {
        *result = &(*json)[key];
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

EXPORT_API(bool, json_get_by_index, (parameterSizeof<json_ptr, int32_t, json_ptr*>()))
(JsonPtr json, int32_t index, JsonPtr* result)
{
    if (!json) {
        return false;
    }

    try {
        *result = &(*json)[index];
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

template <class T>
inline bool jsonGetBaseType(JsonPtr json, T* result)
{
    if (!json) {
        return false;
    }

    try {
        *result = json->get<T>();
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

EXPORT_API(bool, json_get_integer, (parameterSizeof<json_ptr, int64_t*>()))
(JsonPtr json, int64_t* result)
{
    return jsonGetBaseType<int64_t>(json, result);
}

EXPORT_API(bool, json_get_double, (parameterSizeof<json_ptr, double*>()))
(JsonPtr json, double* result)
{
    return jsonGetBaseType<double>(json, result);
}

EXPORT_API(bool, json_get_boolean, (parameterSizeof<json_ptr, bool*>()))
(JsonPtr json, bool* result)
{
    return jsonGetBaseType<bool>(json, result);
}

EXPORT_API(bool, json_get_string, (parameterSizeof<json_ptr, cstr_t*>()))
(JsonPtr json, cstr_t* result)
{
    if (!json) {
        return false;
    }

    try {
        *result = newCstr(json->get<std::string>());
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

EXPORT_API(int32_t, json_size, (parameterSizeof<json_ptr>()))
(JsonPtr json)
{
    try {
        return json ? json->size() : -1;
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return -1;
    }
}

EXPORT_API(bool, json_put, (parameterSizeof<json_ptr, cstr_t, json_ptr>()))
(JsonPtr json, cstr_t key, JsonPtr value)
{
    if (!json) {
        return false;
    }

    try {
        if (value) {
            (*json)[key] = *value;
        } else {
            (*json)[key] = nullptr;
        }
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

template <class T>
inline bool jsonPutBasicType(JsonPtr json, cstr_t key, T value)
{
    if (!json) {
        return false;
    }

    try {
        (*json)[key] = value;
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

EXPORT_API(bool, json_put_string, (parameterSizeof<json_ptr, cstr_t, cstr_t>()))
(JsonPtr json, cstr_t key, cstr_t value)
{
    return jsonPutBasicType(json, key, value);
}

EXPORT_API(bool, json_put_integer, (parameterSizeof<json_ptr, cstr_t, int64_t>()))
(JsonPtr json, cstr_t key, int64_t value)
{
    return jsonPutBasicType(json, key, value);
}

EXPORT_API(bool, json_put_double, (parameterSizeof<json_ptr, cstr_t, double>()))
(JsonPtr json, cstr_t key, double value)
{
    return jsonPutBasicType(json, key, value);
}

EXPORT_API(bool, json_put_boolean, (parameterSizeof<json_ptr, cstr_t, bool>()))
(JsonPtr json, cstr_t key, bool value)
{
    return jsonPutBasicType(json, key, value);
}

EXPORT_API(bool, json_push_back, (parameterSizeof<json_ptr, json_ptr>()))
(JsonPtr json, JsonPtr value)
{
    if (!json) {
        return false;
    }

    try {
        if (value) {
            json->push_back(*value);
        } else {
            json->push_back(nullptr);
        }
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

template <class T>
inline bool jsonPushBackBasicType(JsonPtr json, T value)
{
    if (!json) {
        return false;
    }

    try {
        json->push_back(value);
    } catch (nlohmann::json::exception& e) {
        kLastError = e.what();
        return false;
    }

    return true;
}

EXPORT_API(bool, json_push_back_string, (parameterSizeof<json_ptr, cstr_t>()))
(JsonPtr json, cstr_t value)
{
    return jsonPushBackBasicType(json, value);
}

EXPORT_API(bool, json_push_back_integer, (parameterSizeof<json_ptr, int64_t>()))
(JsonPtr json, int64_t value)
{
    return jsonPushBackBasicType(json, value);
}

EXPORT_API(bool, json_push_back_double, (parameterSizeof<json_ptr, double>()))
(JsonPtr json, double value)
{
    return jsonPushBackBasicType(json, value);
}

EXPORT_API(bool, json_push_back_boolean, (parameterSizeof<json_ptr, bool>()))
(JsonPtr json, bool value)
{
    return jsonPushBackBasicType(json, value);
}

EXPORT_API(void, json_remove_by_index, (parameterSizeof<json_ptr, int32_t>()))
(JsonPtr json, int32_t index)
{
    if (json) {
        json->erase(index);
    }
}

EXPORT_API(void, json_remove_by_key, (parameterSizeof<json_ptr, cstr_t>()))
(JsonPtr json, cstr_t key)
{
    if (json) {
        json->erase(key);
    }
}

EXPORT_API(void, json_get_keys, (parameterSizeof<json_ptr>()))
(JsonPtr json, StringVectorPtr vector)
{
    if (!json || !json->is_object()) {
        return;
    }

    auto itemsIterator = json->items();
    auto distance = std::distance(itemsIterator.begin(), itemsIterator.end());
    *vector = StringVector { distance, new cstr_t[distance] };

    size_t index = 0;
    for (auto& item : itemsIterator) {
        vector->vec[index++] = newCstr(item.key());
    }
}

EXPORT_API(cstr_t, json_last_error, 0)
()
{
    return kLastError;
}
