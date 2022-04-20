#include "basic.hpp"
#include "def_export.hpp"
#include "lepton/include/Lepton.h"
#include "lepton/include/lepton/ParsedExpression.h"
#include "lepton/include/lepton/Parser.h"
#include "nlohmann/json.hpp"
#include <map>
#include <sstream>
#include <string.h>
#include <string>
#include <string_view>

using ParsedExpressionPtr = Lepton::ParsedExpression*;

std::map<std::string, double> parsingJSONParameters(std::string_view json)
{
    std::map<std::string, double> map;

    auto parse = nlohmann::json::parse(json);

    for (auto& element : parse.items()) {
        if (element.value().is_number()) {
            map[element.key()] = element.value().get<double>();
        }
    }
    return map;
}

EXPORT_API(ParsedExpressionPtr, expression_parsed, (parameterSizeof<cstr_t>()))
(cstr_t expr)
{
    try {
        return new Lepton::ParsedExpression { Lepton::Parser::parse(expr) };
    } catch (Lepton::Exception e) {
        return nullptr;
    }
}

EXPORT_API(void, expression_free, (parameterSizeof<MathExpression*>()))
(ParsedExpressionPtr p)
{
    if (p) {
        delete p;
    }
}

EXPORT_API(bool, expression_evaluate, (parameterSizeof<MathExpression*, double>()))
(ParsedExpressionPtr expression, double* result)
{
    if (!expression) {
        return false;
    }

    try {
        *result = expression->evaluate();
    } catch (Lepton::Exception e) {
        return false;
    }

    return true;
}

EXPORT_API(bool, expression_evaluate_with_parameters, (parameterSizeof<MathExpression*, cstr_t, double*>()))
(ParsedExpressionPtr expression, cstr_t parameters, double* result)
{
    if (!expression) {
        return false;
    }

    try {
        *result = expression->evaluate(parsingJSONParameters(parameters));
    } catch (Lepton::Exception e) {
        return false;
    }

    return true;
}

EXPORT_API(ParsedExpressionPtr, expression_optimize, (parameterSizeof<MathExpression*>()))
(ParsedExpressionPtr expression)
{
    if (!expression) {
        return nullptr;
    }

    try {
        return new Lepton::ParsedExpression { expression->optimize() };
    } catch (Lepton::Exception e) {
        return nullptr;
    }
}

EXPORT_API(ParsedExpressionPtr, expression_differentiate, (parameterSizeof<MathExpression*, cstr_t>()))
(ParsedExpressionPtr expression, cstr_t variable)
{
    if (!expression) {
        return nullptr;
    }

    try {
        return new Lepton::ParsedExpression { expression->differentiate(variable) };
    } catch (Lepton::Exception e) {
        return nullptr;
    }
}

EXPORT_API(bool, expression_to_string, (parameterSizeof<MathExpression*, cstr_t>()))
(ParsedExpressionPtr expression, cstr_t *result)
{
    if (!expression) {
        return false;
    }
    try {
        *result = newCstr((std::stringstream() << *expression).str());
    } catch (Lepton::Exception e) {
        return false;
    }

    return true;
}

EXPORT_API(int64_t, quick_pow, (parameterSizeof<int64_t, int32_t>()))
(int64_t x, int32_t n)
{
    if (n == 0) {
        return 1;
    }

    return quick_pow(x * x, n / 2) * ((n & 0x1) ? x : 1);
}
