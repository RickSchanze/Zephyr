#include "Finder.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <fstream>
#include <iostream>

using namespace clang::tooling;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory g_ToolCategory("metareflect options");

void AddIncludePaths(ClangTool &tool) {
  // 读取IncludePath.txt 每一行作为一个字符串
  std::ifstream file(R"(C:\Users\Echo\BaiduSyncDisk\Work\Projects\Zephyr\Reflection\Release\IncludePath.txt)");
  std::string line;
  std::string command;
  while (std::getline(file, line)) {
    const std::string path_command = std::string("-I") + line;
    tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(path_command.c_str()));
  }
}

int main(int argc, const char **argv) {
  /* Parse command-line options. */
  auto optionsParser = CommonOptionsParser::create(argc, argv, g_ToolCategory);
  ClangTool tool(optionsParser->getCompilations(), optionsParser->getSourcePathList());
  MatchFinder finder;
  Finder my_finder;

  /* Search for all fields with an 'annotate' attribute. */
  static DeclarationMatcher const propertyMatcher =
      fieldDecl(decl().bind("id"), isExpansionInMainFile(), hasAttr(clang::attr::Annotate));
  finder.addMatcher(propertyMatcher, &my_finder);

  // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-v", ArgumentInsertPosition::BEGIN));
  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-xc++", ArgumentInsertPosition::BEGIN));
  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-std=c++20", ArgumentInsertPosition::BEGIN));
  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-DREFLECTION", ArgumentInsertPosition::BEGIN));
  AddIncludePaths(tool);
  return tool.run(newFrontendActionFactory(&finder).get());
}

