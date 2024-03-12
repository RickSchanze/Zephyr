/**
 * @file MetaType.h
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#ifndef METATYPE_H
#define METATYPE_H
#include <cstddef>
#include <cstdint>
#include <string>

namespace Reflection {
class Class;

class Type {
public:
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
  Field() noexcept = default;
  Field(const Type *type, const char *name, const uint32_t offset) noexcept
      : m_type(type), m_name(name), m_offset(offset) {}

  const Type *GetType() const noexcept { return m_type; }
  const char *GetName() const noexcept { return m_name; }
  uint32_t GetOffset() const noexcept { return m_offset; }

protected:
  const Type *m_type;
  const char *m_name;
  uint32_t m_offset;
};

class Class : public Type {
public:
  Class() noexcept = default;
  Class(Class *base, Field *field_begin, Field *field_end, const char *name, const uint32_t size) noexcept
      : Type(name, size), m_base_class(base), m_fields(field_begin), m_fields_end(field_end) {}

  Field *GetField(const std::string &name) const {
    for (auto it = m_fields; it != m_fields_end; ++it) {
      if (it->GetName() == name) {
        return it;
      }
    }
    return nullptr;
  }

  template <typename R, typename T>
  R GetValue(T *object, const std::string &name) const {
    auto field = GetField(name);
    return GetValue<R>(object, field);
  }

  template <typename R, typename T>
  R GetValue(T *object, Field *field) const {
    if (object && IsFieldValid(field)) {
      return *reinterpret_cast<R *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset());
    }
    return R();
  }

  template <typename T1, typename T2>
  void SetValue(T1 *object, Field *field, T2 value) const {
    if (object && IsFieldValid(field)) {
      *reinterpret_cast<T2 *>(reinterpret_cast<uintptr_t>(object) + field->GetOffset()) = value;
    }
  }

  template <typename T1, typename T2>
  void SetValue(T1 *object, const std::string &name, T2 value) const {
    auto field = GetField(name);
    SetValue(object, field, value);
  }

  bool IsFieldValid(const Field * field) const {
    if (field == nullptr) return false;
    for (auto it = m_fields; it != m_fields_end; ++it) {
      if (it == field) {
        return true;
      }
    }
    return false;
  }

private:
  Class *m_base_class = nullptr;
  Field *m_fields = nullptr;
  Field *m_fields_end = nullptr;
};

template <typename Type, size_t NFields, size_t NFunctions>
struct ClassBuilder {
  template <typename Lambda>
  explicit ClassBuilder(Lambda &&ctor) noexcept { ctor(this); }

  const size_t num_fields = NFields;
  const size_t num_functions = NFunctions;

  /** 零长数组是未定义行为 */
  Field fields[NFields];
};
template <class T>
const Class *GetClass() noexcept;

template <class T>
const Type *GetType() noexcept;

namespace Detail {
template <class T>
const Class *GetClassImpl() noexcept;
template <class T>
const Type *GetTypeImpl() noexcept;
} // namespace Detail

#define DELCARE_BASE_TYPE(TypeName)                      \
  template <>                                            \
  const Type *GetType<TypeName>() noexcept {             \
    static const Type type(#TypeName, sizeof(TypeName)); \
    return &type;                                        \
  }

DELCARE_BASE_TYPE(int)
DELCARE_BASE_TYPE(double)
DELCARE_BASE_TYPE(float)
DELCARE_BASE_TYPE(char)
DELCARE_BASE_TYPE(unsigned int)
DELCARE_BASE_TYPE(unsigned char)
DELCARE_BASE_TYPE(unsigned short)
DELCARE_BASE_TYPE(unsigned long)
DELCARE_BASE_TYPE(unsigned long long)
DELCARE_BASE_TYPE(long)
DELCARE_BASE_TYPE(long long)
DELCARE_BASE_TYPE(short)
DELCARE_BASE_TYPE(bool)

} // namespace Reflection
#endif
