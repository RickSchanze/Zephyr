/**
 * @file TypeConverter.cpp
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#include "TypeConverter.h"

std::string TypeConverter::ToString(spdlog::string_view_t str)
{
    return {str.begin(), str.end()};
}
