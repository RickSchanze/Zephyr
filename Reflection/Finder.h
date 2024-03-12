/**
 * @file Finder.h
 * @author Echo 
 * @Date 24-3-12
 * @brief 
 */

#ifndef FINDER_H
#define FINDER_H
#include "ReflectedClass.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"


class Finder : public clang::ast_matchers::MatchFinder::MatchCallback {
  using SourceManager = clang::SourceManager;
  using ASTContext = clang::ASTContext;

public:
  using MatchFinder = clang::ast_matchers::MatchFinder;
  void run(const MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

protected:
  void FoundField(const clang::FieldDecl *field);

private:
  std::map<std::string, ReflectedClass> m_classes;
  ASTContext *m_context = nullptr;
  SourceManager *m_sourceman = nullptr;
  std::string m_filename;
};



#endif //FINDER_H
