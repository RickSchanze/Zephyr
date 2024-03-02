/**
 * @file Path.cpp
 * @author Echo
 * @Date 2024/3/3
 * @brief
 */

#include "Path.h"

std::string Path::ExtractFileName(const std::string &path)
{
    return path.find_last_of(ZEPHYR_PATH_SEPARATOR) == std::string::npos ? path : path.substr(path.find_last_of(ZEPHYR_PATH_SEPARATOR) + 1, path.size() - 1);
}
