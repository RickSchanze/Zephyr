/**
 * @file JsonDeserializer.h
 * @author Echo
 * @Date 24-3-25
 * @brief
 */

#pragma once
#include "MetaType.h"
#include "Object/Object.h"

#include "json/value.h"

class Object;
class JsonDeserializer
{

public:
    /**
     * 从一个Json::Value对象序列化一个对象
     * @tparam R 对象的类型，GetClass(ClassTag<R>{})必须已实现 此外必须R必须有默认构造函数
     * @param value 输入对象
     * @return 序列化完成的R对象的指针
     */
    template <typename R>
        requires std::is_base_of_v<Object, R>
    static R *Deserialize(const Json::Value &value);

private:
    /** 反序列化真正的入口 */
    static void InternalDeserialize(const Json::Value &value, void *address, const Reflection::Class *R_class);

    /** 反序列化一个字段 */
    static void InternalDeserializeField(const Json::Value &value, void *address, const Reflection::Field *field);

    /** 反序列化vector */
    static void InternalDeserializeVector(const Json::Value &value, void *address, const Reflection::Field *in_field);

    /** 反序列化指针 */
    static void InternalDeserializePointer(const Json::Value &value, void *address, const Reflection::Field *field);
};

template <typename R>
    requires std::is_base_of_v<Object, R>
R *JsonDeserializer::Deserialize(const Json::Value &value)
{
    const auto *R_class = Reflection::GetClass<R>();
    if (R_class == nullptr)
    {
        return nullptr;
    }
    R *obj = New<R>();
    InternalDeserialize(value, obj, R_class);
    return obj;
}




