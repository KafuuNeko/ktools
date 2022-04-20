#include "var_parser.h"
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>

int main()
{
    std::string str("1. \\[Name: [Name]\\]\n2. \\[HP: [Read: [Name], Node: HP]\\]");
    std::string result;

    std::cout << "parser: \n" << str << std::endl;

    auto r = varParserV1(
        str.cbegin(), str.cend(), std::back_insert_iterator(result), [](std::string_view var, std::string& out) -> bool {
            //std::cout << var << std::endl;

            if (var == "Name") {
                out = "[Kafuu]";
                return true;
            }

            if (var == "Read: [Kafuu], Node: HP") {
                out = "1000";
                return true;
            }

            return false;
        },
        '[', ']');

    std::cout << "\nsuccess: \n" << (r? "true" : "false") << "\n\nresult:\n" << result << std::endl;

    return 0;
}
