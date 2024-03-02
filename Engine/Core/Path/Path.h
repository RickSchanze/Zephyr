/**
 * @file Path.h
 * @author Echo
 * @Date 2024/3/3
 * @brief
 */

#ifndef ZEPHYR_PATH_H
#define ZEPHYR_PATH_H

#ifdef _WIN32
#define ZEPHYR_PATH_SEPARATOR '\\'
#else
#define ZEPHYR_PATH_SEPARATOR '/'
#endif

#include <string>

class Path
{
public:
    static std::string ExtractFileName(const std::string &path);
};

#endif // ZEPHYR_PATH_H
