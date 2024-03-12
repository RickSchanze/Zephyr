/**
 * @file Finder.cpp
 * @author Echo 
 * @Date 24-3-12
 * @brief 
 */
#include "Finder.h"
#include <iostream>
#include <ranges>

using namespace clang;

void Finder::run(const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *decl = Result.Nodes.getNodeAs<FieldDecl>("id");
  m_context = Result.Context;
  m_sourceman = Result.SourceManager;
  if (decl) {
    FoundField(decl);
  }
}
void Finder::onEndOfTranslationUnit() {
  /* For stdout output. */
  /* raw_fd_ostream os(1, false); */
  std::error_code ec;
  llvm::raw_fd_ostream os(m_filename, ec);
  assert(!ec && "error opening file");
  os << "#pragma once\n";
  for (auto &val : m_classes | std::views::values)
    val.Generate(m_context, os);
}

void Finder::FoundField(const clang::FieldDecl *field) {
  m_filename = m_sourceman->getFilename(field->getLocation()).str();
  m_filename.erase(m_filename.end()-2, m_filename.end());
  m_filename.append(".generated.h");
  if (!m_classes.contains(m_filename)) {
    m_classes[m_filename] = ReflectedClass();
    m_classes[m_filename].AddField(field);
  } else {
    m_classes[m_filename].AddField(field);
  }
}
