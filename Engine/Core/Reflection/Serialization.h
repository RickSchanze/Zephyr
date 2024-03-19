#pragma once
#include "../CommonMacro.h"
#include "Interface/ISerializable.h"
#include "MetaType.h"

#define JSON_SERIALIZE_VALUE_FROM_CLASS(type_name)                                                                     \
    if (strcmp(obj_type->GetName(), #type_name) == 0)                                                                  \
    {                                                                                                                  \
        out_json = *static_cast<type_name *>(object);                                                                  \
        return;                                                                                                        \
    }

class JsonSerializer
{
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
        return Serialize(object, t_class, nullptr, out_json);
    }

private:
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
    static void SerializePointer(void *object, const Reflection::Class *obj_class, const Reflection::Field *in_field,
                                 OUT Json::Value &out_json)
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
        Serialize(*ptr, obj_class, in_field, out_json);
    }

    /**
     * 序列化一个vector
     * @param obj vector地址
     * @param vec_class vector模板类型
     * @param in_field 字段
     * @param out_json 输出
     */
    static void SerializeVector(void *obj, const Reflection::ClassTemplate *vec_class,
                                const Reflection::Field *in_field, OUT Json::Value &out_json)
    {
        using namespace Reflection;
        const auto element_type = vec_class->GetTemplateArgsBegin()->type;
        if (element_type == nullptr)
        {
            out_json = Json::nullValue;
            return;
        }
        const auto element_class = static_cast<const Class *>(element_type);
        // 提取出数组的每一个元素
        auto *vec = static_cast<std::vector<char> *>(obj);
        const size_t size = vec->size() * sizeof(char);
        const uint32_t type_size =
            in_field != nullptr && in_field->IsPointer() ? sizeof(std::ptrdiff_t) : element_class->GetSize();
        const uint32_t step_size = type_size / sizeof(char);
        const uint32_t loop_count = size / type_size;
        const auto data = vec->data();
        for (int i = 0; i < loop_count; i++)
        {
            void *vec_element_addr = data + step_size * i;
            Json::Value sub_json;
            if (in_field != nullptr && in_field->IsPointer())
            {
                SerializePointer(vec_element_addr, element_class, in_field, sub_json);
            }
            else
            {
                Serialize(vec_element_addr, element_class, in_field, sub_json);
            }
            out_json.append(sub_json);
        }
    }

    static void SerializeString(void *object, const Reflection::Class *obj_class, const Reflection::Field *in_field,
                                OUT Json::Value &out_json)
    {
        using namespace Reflection;
        if (object == nullptr || obj_class == nullptr)
        {
            out_json = Json::nullValue;
            return;
        }
        const auto *str = static_cast<std::string *>(object);
        out_json = *str;
    }

    /**
     * 序列化任意对象
     * @param object 一个对象
     * @param obj_class object的类型
     * @param in_field 对象的字段
     * @param out_json 输出的json值
     * @return
     */
    static void Serialize(void *object, const Reflection::Class *obj_class, const Reflection::Field *in_field,
                          OUT Json::Value &out_json)
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
            return SerializeVector(object, f_class, in_field, out_json);
        }
        // 序列化std::string
        if (obj_class->GetFlag() == TypeFlag::IsString)
        {
            return SerializeString(object, obj_class,
                      in_field, out_json);
        }
        const auto all_fields = obj_class->GetAllFields();
        for (const auto field : all_fields)
        {
            if (field)
            {
                // 如果是指针类型，那么就调用SerializePointer
                // 这里加判断是因为判断的是Field的类型
                // 而对于类型为vector的field来说,其field的IsPointer标志由vector
                // 的模板类型决定，因此需要在这里再加一层
                if (field->GetType()->GetFlag() == TypeFlag::IsVector)
                {
                    SerializeVector(obj_class->GetFieldAddress(object, field), static_cast<const ClassTemplate *>(field->GetType()),
                              field, out_json[field->GetName()]);
                }
                else
                {
                    if (field->IsPointer())
                    {
                        SerializePointer(obj_class->GetFieldAddress(object, field),
                                         static_cast<const Class *>(field->GetType()), field,
                                         out_json[field->GetName()]);
                    }
                    else
                    {
                        Serialize(obj_class->GetFieldAddress(object, field), static_cast<const Class *>(field->GetType()),
                                  field, out_json[field->GetName()]);
                    }
                }
            }
        }
    }
};
