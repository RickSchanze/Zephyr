/**
* @file ISerializable.h
 * @author Echo
 * @Date 2024/3/14
 * @brief
 */

#ifndef SIMPLEPROJECT_ISERIALIZABLE_H
#define SIMPLEPROJECT_ISERIALIZABLE_H
#include "CommonMacro.h"
#include "json/json.h"

interface ISerializable
{
    public:
    virtual bool Serialize(Json::Value & root) const = 0;
};

#endif // SIMPLEPROJECT_ISERIALIZABLE_H
