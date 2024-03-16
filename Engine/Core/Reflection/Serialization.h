#include "../CommonMacro.h"
#include "Interface/ISerializable.h"
#include "MetaType.h"

#define JSON_SERIALIZE_VALUE_FROM_CLASS(json, type_name, field, class_, object)                                        \
    if (strcmp(field->GetType()->GetName(), #type_name) == 0)                                                          \
    {                                                                                                                  \
        auto value = class_->template GetValue<type_name>(object, field);                                              \
        if (value)                                                                                                     \
        {                                                                                                              \
            json[field->GetName()] = value.value();                                                                    \
            continue;                                                                                                  \
        }                                                                                                              \
    }

/**
 * 序列化任意对象
 * @param object 一个对象
 * @param obj_class object的类型
 * @param out_json 输出的json值
 * @return
 */
bool Serialize(void *object, const Reflection::Class *obj_class, OUT Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || obj_class == nullptr)
    {
        out_json = Json::nullValue;
        return false;
    }
    const auto all_fields = obj_class->GetAllFields();
    for (const auto field : all_fields)
    {
        // 判断Field是不是基础类型
        if (field->GetType()->GetFlag() == TypeFlag::IsBaseType)
        {
            // clang-format off
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, int, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, unsigned int, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, char, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, unsigned char, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, short, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, unsigned short, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, long long, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, unsigned long long, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, float, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, double, field, obj_class, object)
            JSON_SERIALIZE_VALUE_FROM_CLASS(out_json, bool, field, obj_class, object)
            // clang-format on
        }
        // TODO: 编写std::vector<T>的序列化代码，需要考虑T是不是指针
        if (field->GetType()->GetFlag() == TypeFlag::IsVector)
        {
            const auto *f_class = static_cast<const ClassTemplate *>(field->GetType());
        }

        Json::Value sub_json;
        if (!Serialize(obj_class->GetFieldAddress(object, field),
                       static_cast<const Reflection::Class *>(field->GetType()), sub_json))
        {
            sub_json = Json::nullValue;
        }
        out_json[field->GetName()] = sub_json;
    }
    return true;
}

/**
 * 序列化
 * @tparam T
 * @param object 要序列化的object
 * @param out_json 输出的序列化的json
 * @return
 */
template <typename T>
bool Serialize(T *object, OUT Json::Value &out_json)
{
    if (object == nullptr)
    {
        out_json = Json::nullValue;
        return false;
    }
    // 如果对象自己能序列化，那就调用它的序列化方法
    if constexpr (std::is_base_of_v<ISerializable, T>)
    {
        return object->Serialize(out_json);
    }
    // 否则，使用反射进行序列化

    auto *t_class = Reflection::GetClass(object);
    if (t_class == nullptr)
    {
        out_json = Json::nullValue;
        return false;
    }
    return Serialize(object, t_class, out_json);
}
