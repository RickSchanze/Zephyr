/**
 * @file MetaType.h
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#ifndef METATYPE_H
#define METATYPE_H
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stack>
#include <string>
#include <vector>

namespace Reflection {
class Class;
class Type;

template <class T> const Class *GetClass() noexcept;

template <class T> const Type *GetType() noexcept;

template <typename T> const Class *GetClass(T) noexcept { return GetClass<T>(); }

template <typename T> const Type *GetType(T) noexcept { return GetType<T>(); }

class Type {

public:
  template <class T> friend const Type *GetType() noexcept;

  template <class T> friend const Class *GetClass() noexcept;

  Type() noexcept = default;
  Type(const char *name, const uint32_t size) noexcept : m_size(size), m_name(name) {}

  const char *GetName() const noexcept { return m_name; }
  uint32_t GetSize() const noexcept { return m_size; }

private:
  uint32_t m_size = 0;
  const char *m_name = nullptr;
};

class Field {

public:
  template <typename Type, size_t NFields, size_t NFunctions> friend struct ClassBuilder;

  Field() noexcept = default;

  Field(const Type *type, const char *name, const uint32_t offset) noexcept
      : m_type(type), m_name(name), m_offset(offset) {}

  template <typename T> static Field MakeField(const char *name, const uint32_t offset) {
    auto rtn = Field(::Reflection::GetType<T>(), name, offset);
    rtn.SetQualifier<T>();
    return rtn;
  }

  const Type *GetType() const noexcept { return m_type; }
  const char *GetName() const noexcept { return m_name; }
  uint32_t GetOffset() const noexcept { return m_offset; }
  const Class *GetOwnerClass() const noexcept { return m_owner; }

  void SetQualifier(bool is_const, bool is_reference, bool is_pointer) {
    m_is_const = is_const;
    m_is_reference = is_reference;
    m_is_pointer = is_pointer;
  }

  template <typename T> void SetQualifier() {
    SetQualifier(std::is_const_v<T>, std::is_reference_v<T>, std::is_pointer_v<T>);
  }

  bool IsConst() const noexcept { return m_is_const; }
  bool IsReference() const noexcept { return m_is_reference; }
  bool IsPointer() const noexcept { return m_is_pointer; }

private:
  void SetOwnerClass(Class *owner) { m_owner = owner; }

protected:
  const Type *m_type{};
  const char *m_name{};
  Class *m_owner{};
  uint32_t m_offset{};

  bool m_is_const = false;
  bool m_is_reference = false;
  bool m_is_pointer = false;
};

class Class : public Type {
public:
  Class() noexcept = default;
  Class(const Class *base, Field *field_begin, Field *field_end, const char *name, const uint32_t size) noexcept
      : Type(name, size), m_base_class(base), m_fields(field_begin), m_fields_end(field_end) {}

  Field *GetField(const std::string &name) const {
    for (auto it = m_fields; it != m_fields_end; ++it) {
      if (it->GetName() == name) {
        return it;
      }
    }
    return nullptr;
  }

  template <typename R, typename T> std::optional<R> GetValue(T *object, const std::string &name) const {
    auto field = GetField(name);
    return GetValue<R>(object, field);
  }

  template <typename R, typename T> std::optional<R> GetValue(T *object, Field *field) const {
    // 传入object与Field是否有效
    if (!(IsObjectValid(object) && IsFieldValid<R>(field))) {
      return std::nullopt;
    }
    // 要求的返回值类型是否和传入字段的类型一致
    if (GetType<R>() != field->GetType()) {
      return std::nullopt;
    }
    return *reinterpret_cast<R *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset());
  }

  template <typename T1, typename T2> bool SetValue(T1 *object, Field *field, T2 value) const {
    // 传入值是否有效
    if (!(IsObjectValid(object) && IsFieldValid<T2>(field))) {
      return false;
    }
    // 想设置的值的类型和字段的类型是否一致
    if (GetType<T2>() != field->GetType()) {
      return false;
    }
    if (IsObjectValid(object) && IsFieldValid<T2>(field)) {
      *reinterpret_cast<T2 *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset()) = value;
      return true;
    }
    return false;
  }

  template <typename T1, typename T2> bool SetValue(T1 *object, const std::string &name, T2 value) const {
    auto field = GetField(name);
    return SetValue(object, field, value);
  }

  template <typename T> bool IsFieldValid(const Field *field) const {
    if (field == nullptr)
      return false;
    // 检查是否拥有正确的CV限定符号
    if (field->IsConst() && !std::is_const_v<T>) {
      return false;
    }
    if (field->IsReference() && !std::is_reference_v<T>) {
      return false;
    }
    if (field->IsPointer() && !std::is_pointer_v<T>) {
      return false;
    }

    return std::any_of(GetFullFieldTraversal().begin(), GetFullFieldTraversal().end(),
                       [field](const Field *f) { return field == f; });
  }

  // 判断传入对象是不是这个Class
  template <typename T> bool IsObjectValid(T *object) const {
    if (object == nullptr)
      return false;
    if (GetClass<T>() != this) {
      return false;
    }
    return true;
  }

  const Class *GetBaseClass() const { return m_base_class; }

  /**
   * 获取一个可迭代对象，用于遍历生声明在本类（不包括父类的）的字段
   * @return
   */
  std::vector<Field *> &GetFieldTraversal() const {
    static std::vector<Field *> fields;
    if (fields.empty()) {
      for (auto it = m_fields; it != m_fields_end; ++it) {
        fields.push_back(it);
      }
    }
    return fields;
  }

  /**
   * 获取一个可迭代对象，用于遍历生声明在本类（包括父类的）的字段
   * @return
   */
  std::vector<Field *> &GetFullFieldTraversal() const {
    static std::vector<Field *> fields;
    if (fields.empty()) {
      std::stack<const Class *> stack;
      stack.push(this);
      while (stack.top() != nullptr && stack.top()->GetBaseClass() != nullptr) {
        stack.push(stack.top()->GetBaseClass());
      }
      while (!stack.empty()) {
        const auto top = stack.top();
        stack.pop();
        for (auto it = top->m_fields; it != top->m_fields_end; ++it) {
          fields.push_back(it);
        }
      }
    }
    return fields;
  }

private:
  const Class *m_base_class = nullptr;
  Field *m_fields = nullptr;
  Field *m_fields_end = nullptr;
};

template <typename Type, size_t NFields, size_t NFunctions> struct ClassBuilder {
  template <typename Lambda> explicit ClassBuilder(Lambda &&ctor) noexcept { ctor(this); }

  // 类构造器只能声明为函数静态变量，因此不允许拷贝和移动
  ClassBuilder(const ClassBuilder &) = delete;
  ClassBuilder(ClassBuilder &&) = delete;

  const size_t num_fields = NFields;
  const size_t num_functions = NFunctions;

  void SetFieldsOwner(Class *owner) {
    for (size_t i = 0; i < NFields; ++i) {
      fields[i].SetOwnerClass(owner);
    }
  }

  /** 零长数组是未定义行为 */
  Field fields[NFields];
};

namespace Detail {
template <class T> Class *GetClassImpl() noexcept;
template <class T> Type *GetTypeImpl() noexcept;
} // namespace Detail

template <class T> const Class *GetClass() noexcept {
  Class *rtn_class = Detail::GetClassImpl<std::remove_cv_t<std::remove_pointer_t<T>>>();
  return rtn_class;
}

template <class T> const Type *GetType() noexcept {
  Type *type = Detail::GetTypeImpl<std::remove_cv_t<std::remove_pointer_t<T>>>();
  return type;
}

#define DECLARE_BASE_TYPE(TypeName)                                                                                    \
  template <> inline const Type *GetType<TypeName>() noexcept {                                                        \
    static const Type type(#TypeName, sizeof(TypeName));                                                               \
    return &type;                                                                                                      \
  }

DECLARE_BASE_TYPE(int)
DECLARE_BASE_TYPE(double)
DECLARE_BASE_TYPE(float)
DECLARE_BASE_TYPE(char)
DECLARE_BASE_TYPE(unsigned int)
DECLARE_BASE_TYPE(unsigned char)
DECLARE_BASE_TYPE(unsigned short)
DECLARE_BASE_TYPE(unsigned long)
DECLARE_BASE_TYPE(unsigned long long)
DECLARE_BASE_TYPE(long)
   DECLARE_BASE_TYPE(long long)
   DECLARE_BASE_TYPE(short)
   DECLARE_BASE_TYPE(bool)

}// namespace Reflection
#endif
