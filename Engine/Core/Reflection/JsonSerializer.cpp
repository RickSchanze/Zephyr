#include "JsonSerializer.h"

#define UPDATE_FIELD_TYPE_QUALIFIER(type_name, enum_name)                                                              \
    else if (strcmp(in_field->GetType()->GetName(), #type_name) == 0)                                                  \
    {                                                                                                                  \
        updated_field.SetTypeQualifier(Field::TypeQualifier::enum_name);                                               \
    }

#define JSON_SERIALIZE_VALUE_FROM_CLASS(type_name, enum_name)                                                          \
    if (in_field->GetTypeQualifier() == Field::TypeQualifier::enum_name)                                               \
    {                                                                                                                  \
        out_json = *static_cast<type_name *>(object);                                                                  \
        return;                                                                                                        \
    }

void JsonSerializer::SerializePointer(void *object, const Reflection::Field *in_field, Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || in_field == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    const auto *ptr = static_cast<void **>(object);
    Field updated_field = *in_field;
    updated_field.SetFieldQualifier(in_field->GetFieldQualifier() ^ Field::FieldQualifier::FQ_Pointer);
    Serialize(*ptr, &updated_field, out_json);
}

void JsonSerializer::SerializeVector(void *obj, const Reflection::Field *in_field, Json::Value &out_json)
{
    using namespace Reflection;
    if (obj == nullptr || in_field == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    out_json = Json::arrayValue;
    const auto element_class = in_field->GetType();
    // 提取出数组的每一个元素
    auto *vec = static_cast<std::vector<char> *>(obj);
    const size_t size = vec->size() * sizeof(char);
    const uint32_t type_size = in_field->IsPointer() ? sizeof(std::ptrdiff_t) : element_class->GetSize();
    const uint32_t step_size = type_size / sizeof(char);
    const uint32_t loop_count = size / type_size;
    const auto data = vec->data();

    Field updated_field = *in_field;
    // 遍历Vector时已经不再是Vector了，因此新建一个Field并更新其中的信息进行遍历
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

    for (int i = 0; i < loop_count; i++)
    {
        void *vec_element_addr = data + step_size * i;
        Json::Value sub_json;
        if (in_field->IsPointer())
        {
            SerializePointer(vec_element_addr, &updated_field, sub_json);
        }
        else
        {

            Serialize(vec_element_addr, &updated_field, sub_json);
        }
        out_json.append(sub_json);
    }
}

void JsonSerializer::SerializeString(void *object, const Reflection::Field *in_field, Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || in_field == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }
    const auto *str = static_cast<std::string *>(object);
    out_json = *str;
}

void JsonSerializer::Serialize(void *object, const Reflection::Field *in_field, Json::Value &out_json)
{
    using namespace Reflection;
    if (object == nullptr || in_field == nullptr)
    {
        out_json = Json::nullValue;
        return;
    }

    {
        // 序列化基础类型
        JSON_SERIALIZE_VALUE_FROM_CLASS(uint8_t, UInt8)
        JSON_SERIALIZE_VALUE_FROM_CLASS(uint16_t, UInt16)
        JSON_SERIALIZE_VALUE_FROM_CLASS(uint32_t, UInt32)
        JSON_SERIALIZE_VALUE_FROM_CLASS(uint64_t, UInt64)
        JSON_SERIALIZE_VALUE_FROM_CLASS(int8_t, Int8)
        JSON_SERIALIZE_VALUE_FROM_CLASS(int16_t, Int16)
        JSON_SERIALIZE_VALUE_FROM_CLASS(int32_t, Int32)
        JSON_SERIALIZE_VALUE_FROM_CLASS(int64_t, Int64)
        JSON_SERIALIZE_VALUE_FROM_CLASS(float, Float)
        JSON_SERIALIZE_VALUE_FROM_CLASS(double, Double)
        JSON_SERIALIZE_VALUE_FROM_CLASS(bool, Bool)
    }
    // 下面的顺序很重要 必须是序列化容器->序列化指针->序列化其他特殊类型->序列化其他类型
    // 序列化vector
    if (in_field->GetTypeQualifier() == Field::TypeQualifier::Vector)
    {
        SerializeVector(object, in_field, out_json);
        return;
    }
    // 序列化指针
    if (in_field->IsPointer())
    {
        SerializePointer(object, in_field, out_json);
        return;
    }
    // 序列化String
    if (in_field->GetTypeQualifier() == Field::TypeQualifier::String)
    {
        SerializeString(object, in_field, out_json);
        return;
    }

    // 序列化其他类型
    const auto field_class = static_cast<const Class *>(in_field->GetType());
    const auto all_fields = field_class->GetAllFields();
    for (const auto field : all_fields)
    {
        Serialize(field_class->GetFieldAddress(object, field), field, out_json[field->GetName()]);
    }
}
