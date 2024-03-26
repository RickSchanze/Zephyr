/**
 * @file JsonDeserializer.cpp
 * @author Echo
 * @Date 24-3-25
 * @brief
 */

#include "JsonDeserializer.h"

#define UPDATE_FIELD_TYPE_QUALIFIER(type_name, enum_name)                                                              \
    else if (strcmp(in_field->GetType()->GetName(), #type_name) == 0)                                                  \
    {                                                                                                                  \
        updated_field.SetTypeQualifier(Field::TypeQualifier::enum_name);                                               \
    }

#define JSON_DESERIALIZE_FROM_VALUE(type_name, enum_name, value_func)                                                  \
    case Field::TypeQualifier::enum_name:                                                                              \
        *static_cast<type_name *>(address) = static_cast<type_name>(value_func);                                       \
        return;

#define JSON_DESERIALIZE_BASE_TYPE_POINTER(type_name, enum_name)                                                       \
    case Reflection::Field::TypeQualifier::enum_name:                                                                  \
        *static_cast<type_name **>(address) = new type_name;                                                           \
        break;

void JsonDeserializer::InternalDeserialize(const Json::Value &value, void *address, const Reflection::Class *R_class)
{
    using namespace Reflection;
    const auto all_fields = R_class->GetAllFields();
    for (const auto &field : all_fields)
    {
        const auto name = field->GetName();
        const auto field_addr = R_class->GetFieldAddress(address, field);
        const auto type_attr = field->GetTypeQualifier();
        // 对应field并没有在Json里面 则按默认值来
        if (!value.isMember(name))
        {
            continue;
        }
        // 对应field是一个vector
        if (type_attr == Field::TypeQualifier::Vector)
        {
            const auto &json_value = value[name];
            if (!json_value.isArray())
            {
                continue;
            }
            InternalDeserializeVector(json_value, field_addr, field);
            continue;
        }
        // 是指针则调用指针的反序列化
        if (field->IsPointer())
        {
            InternalDeserializePointer(value[name], field_addr, field);
            continue;
        }
        // 不是容器 不是指针则调用普通的反序列化
        InternalDeserializeField(value[name], field_addr, field);
    }
}

void JsonDeserializer::InternalDeserializeField(const Json::Value &value, void *address, const Reflection::Field *field)
{
    using namespace Reflection;
    // 对基础类型进行反序列化
    switch (field->GetTypeQualifier())
    {
    case Field::TypeQualifier::Normal:
        InternalDeserialize(value, address, static_cast<const Class *>(field->GetType()));
        return;
    case Field::TypeQualifier::Vector:
        return;
    case Field::TypeQualifier::String:
        new (address) std::string(value.asCString());
        break;
    case Field::TypeQualifier::Int8:
        *static_cast<int8_t *>(address) = static_cast<int8_t>(value.asInt64());
        return;

        // clang-format off
    JSON_DESERIALIZE_FROM_VALUE(int16_t, Int16, value.asInt64())
    JSON_DESERIALIZE_FROM_VALUE(int32_t, Int32, value.asInt64())
    JSON_DESERIALIZE_FROM_VALUE(int64_t, Int64, value.asInt64())
    JSON_DESERIALIZE_FROM_VALUE(uint8_t, UInt8, value.asUInt64())
    JSON_DESERIALIZE_FROM_VALUE(uint16_t, UInt16, value.asUInt64())
    JSON_DESERIALIZE_FROM_VALUE(uint32_t, UInt32, value.asUInt64())
    JSON_DESERIALIZE_FROM_VALUE(uint64_t, UInt64, value.asUInt64())
    JSON_DESERIALIZE_FROM_VALUE(float, Float, value.asFloat())
    JSON_DESERIALIZE_FROM_VALUE(double, Double, value.asDouble())
    JSON_DESERIALIZE_FROM_VALUE(bool, Bool, value.asBool())
        // clang-format on
    }
}

void JsonDeserializer::InternalDeserializeVector(const Json::Value &value, void *address,
                                                 const Reflection::Field *in_field)
{
    using namespace Reflection;
    // 应该在原地构造一个新的vector 否则就会更改CDO的值
    const bool is_vec_elem_pointer = in_field->IsPointer();
    const size_t vec_elem_size = in_field->GetSize();
    const size_t vec_size = value.size() * vec_elem_size;
    // 预留足够的元素数量
    static_cast<std::vector<int8_t>*>(address)->resize(vec_size);
    void *vec_data_addr = static_cast<std::vector<int8_t> *>(address)->data();
    Field updated_field = *in_field;

    {
        // 遍历时的类型已经不再是Vector了，因此新建一个Field并更新其中的信息进行遍历
        if (strcmp(in_field->GetType()->GetName(), "std::string") == 0)
        {
            updated_field.SetTypeQualifier(Field::TypeQualifier::String);
        }
        UPDATE_FIELD_TYPE_QUALIFIER(uint8_t, UInt8)
        UPDATE_FIELD_TYPE_QUALIFIER(uint16_t, UInt16)
        UPDATE_FIELD_TYPE_QUALIFIER(uint32_t, UInt32)
        UPDATE_FIELD_TYPE_QUALIFIER(uint64_t, UInt64)
        UPDATE_FIELD_TYPE_QUALIFIER(int8_t, Int8)
        UPDATE_FIELD_TYPE_QUALIFIER(int16_t, Int16)
        UPDATE_FIELD_TYPE_QUALIFIER(int32_t, Int32)
        UPDATE_FIELD_TYPE_QUALIFIER(int64_t, Int64)
        UPDATE_FIELD_TYPE_QUALIFIER(float, Float)
        UPDATE_FIELD_TYPE_QUALIFIER(double, Double)
        UPDATE_FIELD_TYPE_QUALIFIER(bool, Bool)
        else
        {
            updated_field.SetTypeQualifier(Field::TypeQualifier::Normal);
        }
    }

    for (Json::Value::ArrayIndex i = 0; i != value.size(); ++i)
    {
        auto i_vec_data_addr = static_cast<int8_t *>(vec_data_addr) + (i * vec_elem_size) / sizeof(int8_t);
        if (is_vec_elem_pointer)
        {
            InternalDeserializePointer(value[i], i_vec_data_addr, &updated_field);
        }
        else
        {
            InternalDeserializeField(value[i], i_vec_data_addr, &updated_field);
        }
    }
}

void JsonDeserializer::InternalDeserializePointer(const Json::Value &value, void *address,
                                                  const Reflection::Field *field)
{
    if (address != nullptr && field != nullptr)
    {
        Reflection::Field updated_field = *field;
        // 为指针分配内存
        switch (field->GetTypeQualifier())
        {
        case Reflection::Field::TypeQualifier::Normal:
            *static_cast<Object**>(address) = New(static_cast<const Reflection::Class*>(field->GetType()));
            break;
        case Reflection::Field::TypeQualifier::Vector:
            // 调用这个函数时field不应该是Vector
            return;
        case Reflection::Field::TypeQualifier::Int8:
            *static_cast<int8_t **>(address) = new int8_t;
            break;
            JSON_DESERIALIZE_BASE_TYPE_POINTER(int16_t, Int16)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(int32_t, Int32)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(int64_t, Int64)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(uint8_t, UInt8)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(uint16_t, UInt16)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(uint32_t, UInt32)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(uint64_t, UInt64)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(float, Float)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(double, Double)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(bool, Bool)
            JSON_DESERIALIZE_BASE_TYPE_POINTER(std::string, String)
        }
        updated_field.SetFieldQualifier(field->GetFieldQualifier() ^ Reflection::Field ::FieldQualifier::FQ_Pointer);
        InternalDeserializeField(value, *static_cast<void**>(address), &updated_field);
    }
}
