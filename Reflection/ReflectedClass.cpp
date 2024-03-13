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
  // 找到父类的名字
  std::string base_class_name = GetBaseClassName();
  // 前向声明需要的类的名字
  if (!base_class_name.empty()) {
    os << "class " << base_class_name << ";\n";
  }
  os << "class " << class_name << ";\n\n";


  size_t filed_count = m_fields.size();
  os << "namespace Reflection {\n";
  os << "namespace Detail{\n\n";
  os << "template <>\n";
  os << "const Class *GetClassImpl<" << class_name << ">() noexcept {\n";
  os << std::vformat("  static ClassBuilder<{}, {}, {}> class_builder([](auto* builder) {{\n",
                     std::make_format_args(class_name, filed_count, 0));
  GeneratedFields(os, class_name);
  os << "  });\n";
  // 生成Class定义
  {
    if (!base_class_name.empty()) {
      os << std::vformat("  static Class cache(GetClass<{}>(), class_builder.fields, class_builder.fields + "
                         "class_builder.num_fields, \"{}\", sizeof({}));\n",
                         std::make_format_args(base_class_name, class_name, class_name));
    } else {
      os << std::vformat("  static Class cache(nullptr, class_builder.fields, class_builder.fields + "
                         "class_builder.num_fields, \"{}\", sizeof({}));\n",
                         std::make_format_args(class_name, class_name));
    }
    os << "  return &cache;\n";
    os << "}\n\n";
    os << "}\n\n";
    // 模板GetClass
    os << "template <>\n";
    os << "const Class *GetClass<" << class_name << ">() noexcept {\n";
    os << "  return Detail::GetClassImpl<" << class_name << ">();\n";
    os << "}\n\n";
    // 模板GetType
    os << "template <>\n";
    os << "const Type* GetType<" << class_name << ">() noexcept {\n";
    os << "return GetClass<" << class_name << ">();\n";
    os << "}\n\n";
  }
  os << "}\n\n";
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

std::string ReflectedClass::GetBaseClassName() const {
  if (!m_record->bases().empty()) {
    const clang::CXXBaseSpecifier *base_specifier = m_record->bases_begin();
    const CXXRecordDecl *base_record_decl = base_specifier->getType()->getAsCXXRecordDecl();
    if (base_specifier) {
      const std::string base_class_name = base_record_decl->getQualifiedNameAsString();
      std::cout << "Base class name: " << base_class_name << "\n";
      return base_class_name;
    }
  }
  return "";
}

std::string ReflectedClass::GetClassName() const{
  if (m_record) {
    return m_record->getQualifiedNameAsString();
  }
  return "";
}
