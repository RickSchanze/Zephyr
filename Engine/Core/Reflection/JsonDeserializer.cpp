/**
 * @file JsonDeserializer.cpp
 * @author Echo
 * @Date 24-3-25
 * @brief
 */

#include "JsonDeserializer.h"

void JsonDeserializer::InternalDeserialize(const Json::Value &value, const Reflection::Class *R_class, void *address)
{
    using namespace Reflection;
    const auto all_fields = R_class->GetAllFields();
    for (const auto &field : all_fields)
    {
        const auto name = field->GetName();
        const auto field_addr = R_class->GetFieldAddress(address, field);
        const auto filed_attr = field->GetFieldQualifier();
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
            InternalDeserializeVector(json_value, R_class, field_addr, field);
        }
    }
}

void JsonDeserializer::InternalDeserializeVector(const Json::Value &value, const Reflection::Class *R_class,
                                                 void *address, const Reflection::Field *field){
    using namespace Reflection;
}
