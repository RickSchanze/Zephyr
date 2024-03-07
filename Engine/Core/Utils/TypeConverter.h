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

class TypeConverter {
public:
  static std::string ToString(spdlog::string_view_t str);

  /** 将一个UTF8编码的string转换成一个UTF16编码的WString */
  static std::wstring UTF82UTF16WString(const std::string &str);
  /** 将一个UTF16编码的WString转换成一个UTF8编码的string */
  static std::string UTF16WString2UTF8(const std::wstring &str);
};

#endif // ZEPHYR_TYPE_CONVERTER_H
