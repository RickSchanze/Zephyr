/**
 * @file MetaType.h
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#ifndef METATYPE_H
#define METATYPE_H
#include "TypeTraits.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stack>
#include <string>
#include <vector>

namespace Reflection
{
class Class;
class Type;

/** 用于处理函数模板偏特化问题 */
template <typename T>
struct ClassTag
{
};

template <typename T>
struct TypeTag
{
};

/** 获取包含反射信息的T的Class */
template <class T>
const Class *GetClass() noexcept;
/** 获取包含反射信息的T的Type 一般用于基础类型 */
template <class T>
const Type *GetType() noexcept;
/** 获取包含反射信息的obj对象类型的Class */
template <typename T>
const Class *GetClass(T) noexcept
{
    return GetClass<T>();
}
/** 获取包含反射信息的obj对象类型的Type,一般用于基础类型 */
template <typename T>
const Type *GetType(T) noexcept
{
    return GetType<T>();
}



namespace Detail
{
/**
 * 获得类反射信息的具体实现,代码生成器必须实现此函数
 * @note 加一层ClassTag是为了实现模板偏特化,比如
 * template <class T>
 * Class* GetClassImpl(ClassTag<std::vector<T>>() noexcept;
 * */
template <class T>
Class *GetClassImpl(ClassTag<T>) noexcept;

/** 获得类反射信息的具体实现,代码生成器必须实现此函数 */
template <class T>
Type *GetTypeImpl(TypeTag<T>) noexcept;
} // namespace Detail

/**
 * 表示一个Type的Flag
 * 只支持有限的模板
 */
enum class TypeFlag : uint8_t
{
    IsBaseType, /** 是一个基础类型，int,float,char... */
    IsClass,    /** 是一个类 */
    IsVector,   /** 是一个Vector */
};

class Type
{

public:
    template <class T>
    friend Class * ::Reflection::Detail::GetClassImpl(ClassTag<T>) noexcept;

    Type() noexcept = default;

    /**
     * 构造Type
     * @param name 类型名
     * @param size 大小
     * @param flag 表示这个类型是一个基础类型、类还是模板
     */
    Type(const char *name, const uint32_t size, const TypeFlag flag = TypeFlag::IsBaseType) noexcept
        : m_size(size), m_flag(flag), m_name(name)
    {
    }

    /** 获得类型名 */
    const char *GetName() const noexcept
    {
        return m_name;
    }
    /** 获得类型大小 */
    uint32_t GetSize() const noexcept
    {
        return m_size;
    }

    /** 获取类型的Flag */
    TypeFlag GetFlag() const noexcept
    {
        return m_flag;
    }

protected:
    uint32_t m_size = 0;
    TypeFlag m_flag = TypeFlag::IsBaseType;
    const char *m_name = nullptr;
};

// TODO: 将bool表示Qualifier变为使用enum表示
class Field
{

public:
    template <typename Type, size_t NFields, size_t NFunctions, size_t NTemplateArgs>
    friend struct ClassBuilder;

    Field() noexcept = default;

    /**
     * 构造字段,没有const reference和pointer信息
     * @param type 字段类型
     * @param name 字段名
     * @param offset 字段类内偏移
     */
    Field(const Type *type, const char *name, const uint32_t offset) noexcept
        : m_type(type), m_name(name), m_offset(offset)
    {
    }

    /**
     * 生成一个字段，并填入const reference和pointer信息
     * @tparam T
     * @param name
     * @param offset
     * @return
     */
    template <typename T>
    static Field MakeField(const char *name, const uint32_t offset)
    {
        auto rtn = Field(::Reflection::GetType<T>(), name, offset);
        // 如果检测出T是一个std::vector,则其Qualifier将会设置为vector元素类型对应的Qualifier
        if constexpr (IsStdVector<T>::value)
        {
            using element_type = typename ExtractStdVectorType<T>::type;
            rtn.SetQualifier<element_type>();
        }
        else
        {
            rtn.SetQualifier<T>();
        }
        return rtn;
    }

    /** 获得这个字段的类型 */
    const Type *GetType() const noexcept
    {
        return m_type;
    }
    /** 获得这个字段的名字 */
    const char *GetName() const noexcept
    {
        return m_name;
    }
    /** 获得这个字段的类内偏移 */
    uint32_t GetOffset() const noexcept
    {
        return m_offset;
    }
    /** 获得这个字段生命的类 */
    const Class *GetOwnerClass() const noexcept
    {
        return m_owner;
    }
    /** 设置字段的const reference pointer信息 */
    void SetQualifier(bool is_const, bool is_reference, bool is_pointer)
    {
        m_is_const = is_const;
        m_is_reference = is_reference;
        m_is_pointer = is_pointer;
    }
    /** 自动设置字段的const reference pointer信息 */
    template <typename T>
    void SetQualifier()
    {
        SetQualifier(std::is_const_v<T>, std::is_reference_v<T>, std::is_pointer_v<T>);
    }
    /** 字段被生命为const? */
    bool IsConst() const noexcept
    {
        return m_is_const;
    }
    /** 字段被生命为reference? */
    bool IsReference() const noexcept
    {
        return m_is_reference;
    }
    /** 字段被生命为pointer? */
    bool IsPointer() const noexcept
    {
        return m_is_pointer;
    }

private:
    void SetOwnerClass(Class *owner)
    {
        m_owner = owner;
    }

protected:
    const Type *m_type{};
    const char *m_name{};
    Class *m_owner{};
    uint32_t m_offset{};

    bool m_is_const = false;
    bool m_is_reference = false;
    bool m_is_pointer = false;
};

class Class : public Type
{
public:
    Class() noexcept = default;

    /**
     * 构造一个类
     * @param base 父类的Class指针
     * @param field_begin 本类字段起始指针
     * @param field_end 本类字段末尾指针
     * @param name 类型名
     * @param size 类型大小
     * @param flag 类型Flag
     */
    Class(const Class *base, Field *field_begin, Field *field_end, const char *name, const uint32_t size,
          TypeFlag flag = TypeFlag::IsClass) noexcept
        : Type(name, size, flag), m_base_class(base), m_fields(field_begin), m_fields_end(field_end)
    {
    }

    /**
     * 获取名为name的字段指针
     * @param name 字段名
     * @return nullptr表示没找到
     */
    Field *GetField(const std::string &name) const
    {
        for (auto it = m_fields; it != m_fields_end; ++it)
        {
            if (it->GetName() == name)
            {
                return it;
            }
        }
        return nullptr;
    }

    /**
     * 获得名为name的字段的值
     * @tparam R 返回类型
     * @tparam T 对象类型
     * @param object 对象的指针
     * @param name 字段的名字
     * @return
     * @note R必须和字段相对应的类型一样，包括const reference和pointer，如果不一致则会返回nullopt
     */
    template <typename R, typename T>
    std::optional<R> GetValue(T *object, const std::string &name) const
    {
        auto field = GetField(name);
        return GetValue<R>(object, field);
    }

    /**
     * 获得对象Field字段的值
     * @tparam R 返回类型
     * @tparam T 对象类型
     * @param object 对象指针
     * @param field 对象字段
     * @return R必须和字段相对应的类型一样，包括const reference和pointer，如果不一致则会返回nullopt, obj必须和本类相对应
     */
    template <typename R, typename T>
    std::optional<R> GetValue(T *object, Field *field) const
    {
        // 传入object与Field是否有效
        if (!(IsObjectValid(object) && IsFieldValid<R>(field)))
        {
            return std::nullopt;
        }
        // 要求的返回值类型是否和传入字段的类型一致
        if (GetType<R>() != field->GetType())
        {
            return std::nullopt;
        }
        return *reinterpret_cast<R *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset());
    }

    /**
     * 获得对象Field字段的值
     * @tparam R 返回类型
     * @param object 对象
     * @param field 字段指针
     * @return
     * @note void类型的特殊重载 obj不要求与本类对应,但是最好还是对应上;重载这个是因为序列化代码遇到了一些类型擦除的问题
     */
    template <typename R>
    std::optional<R> GetValue(void *object, Field *field) const
    {
        if (object == nullptr || !IsFieldValid<R>(field))
        {
            return {};
        }
        return *reinterpret_cast<R *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset());
    }

    /**
     * 设置一个值
     * @tparam T1 对象类型
     * @tparam T2 值类型
     * @param object 对象指针
     * @param field 字段指针
     * @param value 值
     * @return 成功true 失败false
     * @note obj必须和本类相对应 Field必须和value类型相对应
     */
    template <typename T1, typename T2>
    bool SetValue(T1 *object, Field *field, T2 value) const
    {
        // 传入值是否有效
        if (!(IsObjectValid(object) && IsFieldValid<T2>(field)))
        {
            return false;
        }
        // 想设置的值的类型和字段的类型是否一致
        if (GetType<T2>() != field->GetType())
        {
            return false;
        }
        if (IsObjectValid(object) && IsFieldValid<T2>(field))
        {
            *reinterpret_cast<T2 *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset()) = value;
            return true;
        }
        return false;
    }

    /**
     * 设置一个值
     * @tparam T1 对象类型 必须与本类对应
     * @tparam T2 值类型
     * @param object 对象指针
     * @param name 字段名
     * @param value 值
     * @return 成功true 失败false
     * @note obj必须和本类相对应 value必须和name的字段类型对应，且name对应的字段必须存在
     */
    template <typename T1, typename T2>
    bool SetValue(T1 *object, const std::string &name, T2 value) const
    {
        auto field = GetField(name);
        return SetValue(object, field, value);
    }

    /**
     * 判断T是不是Field的有效类型
     * @tparam T 类型
     * @param field 字段
     * @return 如果T表示的类型就是Field所表示的类型,返回true
     * @note 判断时包括了 const reference和pointer
     */
    template <typename T>
    bool IsFieldValid(const Field *field) const
    {
        if (field == nullptr)
            return false;
        // 检查是否拥有正确的CV限定符号
        if (field->IsConst() && !std::is_const_v<T>)
        {
            return false;
        }
        if (field->IsReference() && !std::is_reference_v<T>)
        {
            return false;
        }
        if (field->IsPointer() && !std::is_pointer_v<T>)
        {
            return false;
        }

        auto all_fields = GetAllFields();
        return std::any_of(all_fields.begin(), all_fields.end(), [field](const Field *f) { return field == f; });
    }

    /**
     * 判断传入对象对象是不是和本类描述的类型一致
     * @tparam T object的类型
     * @param object 对象指针
     * @return 传入对象对象是不是和本类描述的类型一致,返回true
     */
    template <typename T>
    bool IsObjectValid(T *object) const
    {
        if (object == nullptr)
            return false;
        if (GetClass<T>() != this)
        {
            return false;
        }
        return true;
    }

    /**
     * 获取基类指针
     * @return 无基类返回nullptr
     */
    const Class *GetBaseClass() const
    {
        return m_base_class;
    }

    /**
     * 获取一个字段的地址
     * @param object 对象的地址
     * @param field 字段
     * @return 返回object + field->GetOffset()
     */
    void *GetFieldAddress(void *object, const Field *field) const
    {
        return reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset());
    }

    /**
     * 获取一个可迭代对象，用于遍历生声明在本类（不包括父类的）的字段
     * @return
     * TODO: 编写专用迭代器而不是使用vector
     */
    std::vector<Field *> GetFields() const
    {
        std::vector<Field *> fields;
        fields.clear();

        for (auto it = m_fields; it != m_fields_end; ++it)
        {
            fields.push_back(it);
        }

        return fields;
    }

    /**
     * 获取一个可迭代对象，用于遍历生声明在本类（包括父类的）的字段
     * @return
     * TODO: 编写专用迭代器而不是使用vector
     */
    std::vector<Field *> GetAllFields() const
    {
        // 生命静态变量防止生命周期结束
        std::vector<Field *> fields;
        // 清空但不能delete 因为这写Field都是存在Class里的
        // 向父级一层层获得Field
        std::stack<const Class *> stack;
        stack.push(this);
        while (stack.top() != nullptr && stack.top()->GetBaseClass() != nullptr)
        {
            stack.push(stack.top()->GetBaseClass());
        }
        while (!stack.empty())
        {
            const auto top = stack.top();
            stack.pop();
            for (auto it = top->m_fields; it != top->m_fields_end; ++it)
            {
                fields.push_back(it);
            }
        }
        return fields;
    }

private:
    const Class *m_base_class = nullptr;
    Field *m_fields = nullptr;
    Field *m_fields_end = nullptr;
};

struct TemplateArgument
{
    const Type *type;
};

class ClassTemplate : public Class
{
public:
    ClassTemplate(const Class *base, Field *field_begin, Field *field_end, const char *name, const uint32_t size,
                  const TypeFlag flag, TemplateArgument *template_begin, TemplateArgument *template_end)
        : Class(base, field_begin, field_end, name, size, flag), m_t_args_begin(template_begin),
          m_t_args_end(template_end)
    {
    }

    TemplateArgument *GetTemplateArgsBegin() const
    {
        return m_t_args_begin;
    }

protected:
    TemplateArgument *m_t_args_begin;
    TemplateArgument *m_t_args_end;
};

/** 用于存储类的字段等信息的代码 */
template <typename Type, size_t NFields, size_t NFunctions, size_t NTemplateArgs = 0>
struct ClassBuilder
{
    template <typename Lambda>
    explicit ClassBuilder(Lambda &&ctor) noexcept
    {
        ctor(this);
    }

    const size_t num_fields = NFields;
    const size_t num_functions = NFunctions;
    const size_t num_template_args = NTemplateArgs;

    void SetFieldsOwner(Class *owner)
    {
        for (size_t i = 0; i < NFields; ++i)
        {
            fields[i].SetOwnerClass(owner);
        }
    }

    /** 零长数组是未定义行为 */
    /** 所有类字段 */
    Field fields[NFields + 1];
    /** 所有模板参数 */
    TemplateArgument template_args[NFields + 1];
};

#include "BaseType.h"
#include "TemplateType.h"

template <class T>
const Class *GetClass() noexcept
{
    using type = std::remove_cv_t<std::remove_pointer_t<T>>;
    const Class *rtn_class = Detail::GetClassImpl<>(ClassTag<type>{});
    return rtn_class;
}

template <class T>
const Type *GetType() noexcept
{
    using type = std::remove_cv_t<std::remove_pointer_t<T>>;
    const Type *rtn_type = Detail::GetTypeImpl(TypeTag<type>{});
    return rtn_type;
}

} // namespace Reflection

// #include "TemplateType.h"

// #include "TemplateType.h"
#endif
