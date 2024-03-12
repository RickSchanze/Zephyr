/**
 * @file ReflectedClass.cpp
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#include "ReflectedClass.h"

#include <format>
#include <iostream>
void ReflectedClass::Generate(const clang::ASTContext *context, llvm::raw_fd_ostream &os) const {
  std::string class_name = m_record->getQualifiedNameAsString();

  size_t filed_count = m_fields.size();
  os << "namespace Reflection {\n";
  os << "namespace Detail{\n";
  os << "template <>\n";
  os << "const Class *GetClassImpl<" << class_name << ">() noexcept {\n";
  os << std::vformat("  static ClassBuilder<{}, {}, {}> class_builder([](auto* builder) {{\n",
                     std::make_format_args(class_name, filed_count, 0));
  GeneratedFields(os, class_name);
  os << "  });\n";
  // 生成Class定义
  os << std::vformat("  static Class cache(nullptr, class_builder.fields, class_builder.fields + "
                     "class_builder.num_fields, \"{}\", sizeof({}));\n",
                     std::make_format_args(class_name, class_name));
  os << "return &cache;\n";
  os << "}\n";
  os << "}\n";
  os << "template <>\n";
  os << "const Class *GetClass<" << class_name << ">() noexcept {\n";
  os << "  return Detail::GetClassImpl<" << class_name << ">();\n";
  os << "}\n";
  os << "}\n";
}

void ReflectedClass::GeneratedFields(llvm::raw_fd_ostream &os, std::string class_name) const {
  for (int i = 0; i < m_fields.size(); i++) {
    auto *field = m_fields[i];
    std::string field_name = field->getNameAsString();
    std::string field_type_name = field->getType().getAsString();
    os << std::vformat("    builder->fields[{}] = Field(GetType<{}>(), \"{}\", offsetof({}, {}));\n",
                       std::make_format_args(i, field_type_name, field_name, class_name, field_name));
  }
}
