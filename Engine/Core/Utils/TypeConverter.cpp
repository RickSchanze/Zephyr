/**
 * @file TypeConverter.cpp
 * @author Echo
 * @Date 2024/2/29
 * @brief
 */

#include "TypeConverter.h"

#include "utf8.h"

std::string TypeConverter::ToString(spdlog::string_view_t str) {
  return {str.begin(), str.end()};
}

std::wstring TypeConverter::UTF82UTF16WString(const std::string &str) {
  std::wstring rtn;
  utf8::utf8to16(str.begin(), str.end(), std::back_inserter(rtn));
  return rtn;
}

std::string TypeConverter::UTF16WString2UTF8(const std::wstring &str) {
  std::string rtn;
  utf8::utf16to8(str.begin(), str.end(), std::back_inserter(rtn));
  return rtn;
}
