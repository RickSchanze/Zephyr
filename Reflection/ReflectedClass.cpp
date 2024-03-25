/**
 * @file ReflectedClass.cpp
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#include "ReflectedClass.h"

#include <format>
#include <iostream>
// TODO: 修改生成代码适配
void ReflectedClass::Generate(const clang::ASTContext *context, llvm::raw_fd_ostream &os) const
{
    std::string class_name = m_record->getQualifiedNameAsString();
    // 找到父类的名字
    std::string base_class_name = GetBaseClassName();
    // 前向声明需要的类的名字
    if (!base_class_name.empty())
    {
        os << "class " << base_class_name << ";\n";
    }
    os << "class " << class_name << ";\n\n";

    size_t filed_count = m_fields.size();
    os << "namespace Reflection {\n";
    os << "namespace Detail{\n\n";
    os << "template <>\n";
    os << "Class *GetClassImpl(ClassTag<" << class_name << ">) noexcept {\n";
    os << std::vformat("static ClassBuilder<{}, {}, {}> class_builder([](auto* builder) {{\n",
                       std::make_format_args(class_name, filed_count, 0));

    GeneratedFields(os, class_name);

    os << "});\n";
    // 生成Class定义
    {
        if (!base_class_name.empty())
        {
            os << std::vformat("static Class cache(GetClass<{}>(), class_builder.fields, class_builder.fields + "
                               "class_builder.num_fields, \"{}\", sizeof({}));\n",
                               std::make_format_args(base_class_name, class_name, class_name));
        }
        else
        {
            os << std::vformat("static Class cache(nullptr, class_builder.fields, class_builder.fields + "
                               "class_builder.num_fields, \"{}\", sizeof({}));\n",
                               std::make_format_args(class_name, class_name));
        }
        os << "class_builder.SetFieldsOwner(&cache);\n";
        os << "Object::SetClassDefaultObject(&cache, new " << class_name << "());\n";
        os << "return &cache;\n";
        os << "}\n\n";
        // 模板GetTypeImpl
        os << "template <>\n";
        os << "Type *GetTypeImpl(TypeTag<" << class_name << ">) noexcept {\n";
        os << "return GetClassImpl(ClassTag<" << class_name << ">{});\n";
        os << "}\n\n";

        os << "// register this type cdo to object\n";
        os << "const Class* _init_" << class_name << " = GetClass<" << class_name << ">();\n";
    }
    os << "}\n";
    os << "}\n\n";
}

void ReflectedClass::GeneratedFields(llvm::raw_fd_ostream &os, std::string class_name) const
{
    static std::map<std::string, std::string> bad_basic_type_name = {
        {"char", "int8_t"},
        {"short", "int16_t"},
        {"int", "int32_t"},
        {"long", "int64_t"},
        {"long long", "int64_t"},

        {"unsigned char", "uint8_t"},
        {"short", "uint16_t"},
        {"unsigned int", "uint32_t"},
        {"unsigned long", "uint64_t"},
        {"unsigned long long", "uint64_t"},
    };
    for (int i = 0; i < m_fields.size(); i++)
    {
        auto *field = m_fields[i];
        std::string field_name = field->getNameAsString();
        std::string field_type_name = field->getType().getAsString();
        if (HasReference(field_type_name))
        {
            std::cerr << "Error: " << field_name << " has reference type which not supported yet" << std::endl;
            exit(1);
        }
        std::string modified_field_type_name = RemovePointerName(RemoveConstName(field_type_name));
        if (bad_basic_type_name.contains(modified_field_type_name))
        {
            std::cerr << "Error: please dont use \"" << field_type_name << "\" directly, use \""
                      << bad_basic_type_name[field_type_name] << "\" instead" << std::endl;
            exit(1);
        }
        os << std::vformat("builder->fields[{}] = Field::MakeField<{}>(\"{}\", offsetof({}, {}));\n",
                           std::make_format_args(i, field_type_name, field_name, class_name, field_name));
    }
}

std::string ReflectedClass::GetBaseClassName() const
{
    if (!m_record->bases().empty())
    {
        const clang::CXXBaseSpecifier *base_specifier = m_record->bases_begin();
        const CXXRecordDecl *base_record_decl = base_specifier->getType()->getAsCXXRecordDecl();
        if (base_specifier)
        {
            const std::string base_class_name = base_record_decl->getQualifiedNameAsString();
            return base_class_name;
        }
    }
    return "";
}

std::string ReflectedClass::GetClassName() const
{
    if (m_record)
    {
        return m_record->getQualifiedNameAsString();
    }
    return "";
}

std::string ReflectedClass::RemoveConstName(const std::string &type_name) const
{
    std::string result = type_name;
    if (result.find("const") != std::string::npos)
    {
        result.erase(result.find("const"), 6);
    }
    return result;
}

std::string ReflectedClass::RemovePointerName(const std::string &type_name) const
{
    std::string result = type_name;
    if (result.find(" *") != std::string::npos)
    {
        result.erase(result.find(" *"), 2);
    }
    return result;
}

bool ReflectedClass::HasReference(const std::string &type_name) const
{
    if (type_name.find('&') != std::string::npos)
    {
        return true;
    }
    return false;
}
