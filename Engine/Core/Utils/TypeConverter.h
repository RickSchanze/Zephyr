/**
 * @file TypeConverter.h
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#ifndef ZEPHYR_TYPE_CONVERTER_H
#define ZEPHYR_TYPE_CONVERTER_H

#include <string>

#include "spdlog/common.h"

class TypeConverter
{
public:
    static std::string ToString(spdlog::string_view_t str);
};

#endif // ZEPHYR_TYPECONVERTER_H
