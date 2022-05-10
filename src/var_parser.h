#pragma once

/**
 * @file var_parser.hpp
 * @author kafuu (kafuuneko@gmail.com)
 * @brief 文本变量处理
 * @version 0.1
 * @date 2022-04-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <stack>
#include <stdint.h>
#include <string>
#include <type_traits>
#include <vector>

enum class CharacterTypes : uint8_t {
    Normal,
    LeftBracket
};

struct Character {
    char ch;
    CharacterTypes type;
};

inline char characterEscaping(char ch) noexcept
{
    switch (ch) {
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    default:
        return ch;
    }
}

template <class InputIterator, class OutputIterator, class Callback>
static bool varParserV1(
    InputIterator first, InputIterator last, OutputIterator output, Callback callback, char leftBracket, char rightBracket,
    std::enable_if_t<
        std::is_same_v<
            std::remove_cv_t<
                std::remove_reference_t<decltype(*(InputIterator {}))>>,
            char>,
        int> = 0)
{
    std::vector<char> buffer;
    std::stack<Character> character;

    for (auto inputIterator = first; inputIterator != last; ++inputIterator) {

        auto element = *inputIterator;

        if (element == leftBracket) {
            character.push(Character { element, CharacterTypes::LeftBracket });
            continue;
        }

        if (element == rightBracket) {
            bool leftBracketExists = false;

            //当遇到右括号，将不断弹出栈中内容到buffer，直到弹出左括号停止。如果直到栈空也未遇到左括号，则解析失败
            buffer.clear();
            while (!character.empty()) {
                auto [topChar, topType] = character.top();

                character.pop();

                if (topType == CharacterTypes::LeftBracket) {
                    //遭遇左括号，调用回调取得变量解析结果
                    leftBracketExists = true;

                    //回调结果暂存
                    std::string cbr;

                    if (callback(std::string(buffer.rbegin(), buffer.rend()).c_str(), cbr)) {
                        //有变量解析结果，将回调结果重新入栈
                        for (char c : cbr) {
                            character.push(Character { c, CharacterTypes::Normal });
                        }

                    } else {
                        //无变量解析结果，还原此变量字符串
                        character.push(Character { leftBracket, CharacterTypes::Normal });
                        for (auto j = buffer.rbegin(); j != buffer.rend(); ++j) {
                            character.push(Character { *j, CharacterTypes::Normal });
                        }
                        character.push(Character { rightBracket, CharacterTypes::Normal });
                    }

                    break;
                }

                buffer.push_back(topChar);
            }

            //遇到了右括号，但在这之前并无左括号，解析错误
            if (!leftBracketExists) {
                return false;
            }

            continue;
        }

        if (element == '\\') {
            ++inputIterator;
            if (inputIterator == last) {
                return false;
            }
            element = characterEscaping(*inputIterator);
        }

        character.push(Character { element, CharacterTypes::Normal });
    }

    //将栈内容输出到buffer
    buffer.clear();
    while (!character.empty()) {
        if (character.top().type != CharacterTypes::Normal) {
            return false;
        }
        buffer.push_back(character.top().ch);
        character.pop();
    }

    //将buffer中内容输出
    std::copy(buffer.rbegin(), buffer.rend(), output);

    return true;
}
