#include "var_parser.h"
#include "basic.hpp"
#include "def_export.hpp"
#include <iterator>
#include <memory>
#include <string.h>
#include <string>


typedef cstr_t (*__stdcall CallbackT)(cstr_t, void*);

EXPORT_API(bool, var_parser_v1, (parameterSizeof<cstr_t>()))
(cstr_t input, cstr_t* output, CallbackT callback, void* param, char leftBracket, char rightBracket)
{
    std::string temp;

    auto r = varParserV1(
        input, input + strlen(input), std::back_insert_iterator(temp), [param, callback](const std::string& var, std::string& rt) {
            std::unique_ptr<const char[]> rb(callback(var.c_str(), param));

            if (rb) {
                rt = std::string(rb.get());
            }

            return static_cast<bool>(rb);
        },
        leftBracket, rightBracket);

    if (r) {
        *output = newCstr(temp);
    }

    return r;
}
