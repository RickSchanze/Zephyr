#pragma once
#include "../CommonMacro.h"
#include "MetaType.h"
#include "json/json.h"

namespace Json
{
class Value;
}
class JsonSerializer
{
    friend class Reflection::Field;

public:
    /**
     * 序列化
     * @tparam T
     * @param object 要序列化的object
     * @param out_json 输出的序列化的json
     * @return
     */
    template <typename T>
    static void Serialize(T *object, OUT Json::Value &out_json)
    {
        if (object == nullptr)
        {
            out_json = Json::nullValue;
            return;
        }
        auto *t_class = Reflection::GetClass(object);
        if (t_class == nullptr)
        {
            out_json = Json::nullValue;
            return;
        }

        const auto all_fields = t_class->GetAllFields();
        for (const auto field : all_fields)
        {
            Serialize(t_class->GetFieldAddress(object, field), field, out_json[field->GetName()]);
        }
    }

private:
    /**
     * 序列化指针类型的变量 具体想法就是将object*转换为void** 看是不是空
     * 如果是空，那么就是null
     * 如果不是那么把指针指向对象的地址再传递给Serlize
     * @param object 字段的地址
     * @param in_field 字段的具体信息
     * @param out_json 输出json
     */
    static void SerializePointer(void *object, const Reflection::Field *in_field, OUT Json::Value &out_json);

    /**
     * 序列化一个vector
     * @param obj vector地址
     * @param in_field 字段
     * @param out_json 输出
     */
    static void SerializeVector(void *obj, const Reflection::Field *in_field, OUT Json::Value &out_json);


    /**
     * 序列化String
     * @param object 对象地址 此对象是一个std::string
     * @param in_field 字段信息
     * @param out_json 输出json
     */
    static void SerializeString(void *object, const Reflection::Field *in_field, OUT Json::Value &out_json);

    /**
     * 序列化任意对象
     * @param object 对象地址
     * @param in_field 对象的字段信息
     * @param out_json 输出的json值
     * @return
     */
    static void Serialize(void *object, const Reflection::Field *in_field, OUT Json::Value &out_json);
};
