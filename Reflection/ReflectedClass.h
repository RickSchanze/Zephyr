/**
 * @file ReflectedClass.h
 * @author Echo
 * @Date 24-3-12
 * @brief
 */

#ifndef REFLECTEDCLASS_H
#define REFLECTEDCLASS_H

#include <clang/AST/DeclCXX.h>

class ReflectedClass {
  using CXXRecordDecl = clang::CXXRecordDecl;
  using FieldDecl = clang::FieldDecl;
  using FunctionDecl = clang::FunctionDecl;

public:
  ReflectedClass() = default;
  explicit ReflectedClass(const clang::CXXRecordDecl *record) : m_record(record) {}

  void AddField(const FieldDecl *field) { m_fields.push_back(field); }

  void Generate(const clang::ASTContext *context, llvm::raw_fd_ostream &os) const;

protected:
  void GeneratedFields(llvm::raw_fd_ostream &os, std::string class_name) const;

private:
  const clang::CXXRecordDecl *m_record = nullptr;
  std::vector<const FieldDecl *> m_fields;
  std::vector<const FunctionDecl *> m_functions;
};

#endif // REFLECTEDCLASS_H
