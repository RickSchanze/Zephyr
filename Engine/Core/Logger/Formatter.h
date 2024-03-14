/**
 * @file Formatter.h
 * @author Echo
 * @Date 24-3-5
 * @brief 为自定义类型提供格式化输出
 */

#ifndef FORMATTER_H
#define FORMATTER_H

#include <filesystem>
#include <format>

#include "utf8.h"

// fomatter for std::filesystem::path
template <>
struct std::formatter<std::filesystem::path> : std::formatter<std::string>
{
    auto format(const std::filesystem::path &my, format_context &ctx) const -> decltype(ctx.out())
    {
        return std::format_to(ctx.out(), "{}", my.string());
    }
};

template <>
struct std::formatter<std::wstring> : std::formatter<std::string>
{
    auto format(const std::wstring &my, format_context &ctx) const -> decltype(ctx.out())
    {
        std::string str;
        utf8::utf16to8(my.begin(), my.end(), std::back_inserter(str));
        return std::format_to(ctx.out(), "{}", str);
    }
};
#endif //FORMATTER_H
