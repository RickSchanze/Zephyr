/**
 * @file Guid.h
 * @author Echo 
 * @Date 2024/2/27
 * @brief 生成GUID 目前只可用在Windows
 * TODO: 多平台支持
 */

#ifndef ZEPHYR_GUID_H
#define ZEPHYR_GUID_H

#include <experimental/string>
#include <objbase.h>

class Guid
{
public:
    /** 随机生成GUID */
    Guid();
    /** 释放GUID */
    ~Guid() = default;
    /** 从GUID获取string */
    std::string ToString() const;

private:
    GUID m_guid{};
    inline static const int GUID_LENGTH = 64;
};

#endif//ZEPHYR_GUID_H
