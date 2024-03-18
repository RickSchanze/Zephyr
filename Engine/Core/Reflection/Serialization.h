#pragma once
#include "../CommonMacro.h"
#include "Interface/ISerializable.h"
#include "MetaType.h"
#include "Reflection/TemplateType.h"

#define JSON_SERIALIZE_VALUE_FROM_CLASS(type_name)                                                                     \
    if (strcmp(obj_type->GetName(), #type_name) == 0)                                                                  \
    {                                                                                                                  \
        out_json = *static_cast<type_name *>(object);                                                                  \
        return;                                                                                                        \
    }

void Serialize(void *object, const Reflection::Class *obj_class, OUT Json::Value &out_json);

/**
 * 序列化一个vector
 * @param obj vector地址
 * @param vec_class vector模板类型
 * @param out_json 输出
 */
static void SerializeVector(void *obj, const Reflection::ClassTemplate *vec_class, OUT Json::Value &out_json)
{
    using namespace Reflection;
    const auto element_type = vec_class->GetTemplateArgsBegin()->type;
    if (element_type == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    const auto element_class = static_cast<const Class *>(element_type);
    auto container_t_class = static_cast<const Class *>(vec_class);
    // 提取出数组的每一个元素
    auto *vec = static_cast<std::vector<char> *>(obj);
    size_t size = vec->size() * sizeof(char);
    uint32_t type_size = element_class->GetSize();
    uint32_t step_size = type_size / sizeof(char);
    uint32_t loop_count = size / type_size;
    auto data = vec->data();
    for (int i = 0; i < loop_count; i++)
    {
        void *vec_element_addr = data + step_size * i;
        Json::Value sub_json;
        Serialize(vec_element_addr, element_class, sub_json);
        out_json.append(sub_json);
    }
}

/**
 * 序列化基础类型
 * @param object 基础类型变量地址
 * @param obj_type 类型
 * @param out_json 输出元素
 */
static void SerializeBaseType(void *object, const Reflection::Type *obj_type, OUT Json::Value &out_json)
{
    using namespace Reflection;
    if (obj_type == nullptr || obj_type->GetFlag() != TypeFlag::IsBaseType)
    {
        out_json = Json::nullValue;
        return;
    }
    JSON_SERIALIZE_VALUE_FROM_CLASS(int)
    JSON_SERIALIZE_VALUE_FROM_CLASS(double)
    JSON_SERIALIZE_VALUE_FROM_CLASS(float)
    JSON_SERIALIZE_VALUE_FROM_CLASS(short)
    JSON_SERIALIZE_VALUE_FROM_CLASS(char)
    JSON_SERIALIZE_VALUE_FROM_CLASS(unsigned int)
    JSON_SERIALIZE_VALUE_FROM_CLASS(unsigned short)
    JSON_SERIALIZE_VALUE_FROM_CLASS(unsigned char)
    JSON_SERIALIZE_VALUE_FROM_CLASS(bool)
    JSON_SERIALIZE_VALUE_FROM_CLASS(long long)
    JSON_SERIALIZE_VALUE_FROM_CLASS(unsigned long long)
}

// 序列化指针类型的变量 具体想法就是将object*转换为void** 看是不是空
// 如果是空，那么就是null
// 如果不是那么把指针指向对象的地址再传递给Serlize
static void SerializePointer(void *object, const Reflection::Class *obj_class, OUT Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || obj_class == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    const auto *ptr = static_cast<void **>(object);
    if (*ptr == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    Serialize(*ptr, obj_class, out_json);
}

/**
 * 序列化任意对象
 * @param object 一个对象
 * @param obj_class object的类型
 * @param out_json 输出的json值
 * @return
 */
void Serialize(void *object, const Reflection::Class *obj_class, OUT Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || obj_class == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    // obj_class是基础类型
    if (obj_class->GetFlag() == TypeFlag::IsBaseType)
    {
        return SerializeBaseType(object, obj_class, out_json);
    }
    // obj_class是一个vector
    if (obj_class->GetFlag() == TypeFlag::IsVector)
    {
        const auto *f_class = static_cast<const ClassTemplate *>(obj_class);
        return SerializeVector(object, f_class, out_json);
    }
    const auto all_fields = obj_class->GetAllFields();
    for (const auto field : all_fields)
    {
        if (field)
        {
            if (field->IsPointer())
            {
                SerializePointer(obj_class->GetFieldAddress(object, field),
                                 static_cast<const Class *>(field->GetType()), out_json[field->GetName()]);
            }
            else
            {
                Serialize(obj_class->GetFieldAddress(object, field), static_cast<const Class *>(field->GetType()),
                          out_json[field->GetName()]);
            }
        }
    }
}

/**
 * 序列化
 * @tparam T
 * @param object 要序列化的object
 * @param out_json 输出的序列化的json
 * @return
 */
template <typename T>
void Serialize(T *object, OUT Json::Value &out_json)
{
    if (object == nullptr)
    {
        out_json = Json::nullValue;
        return;
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
        return;
    }
    return Serialize(object, t_class, out_json);
}
