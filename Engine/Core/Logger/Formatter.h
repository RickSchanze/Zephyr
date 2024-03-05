/**
 * @file Formatter.h
 * @author Echo 
 * @Date 24-3-5
 * @brief 为自定义类型提供格式化输出
 */

#ifndef FORMATTER_H
#define FORMATTER_H

#include <spdlog/fmt/fmt.h>
#include <filesystem>

// fomatter for std::filesystem::path
template<>
struct fmt::formatter<std::filesystem::path> : fmt::formatter<std::string>
{
    auto format(const std::filesystem::path& my, format_context &ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}", my.string());
    }
};

#endif //FORMATTER_H
