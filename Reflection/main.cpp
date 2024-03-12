#include "Finder.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang::tooling;
using namespace clang::ast_matchers;


static llvm::cl::OptionCategory g_ToolCategory("metareflect options");

int main(int argc, const char **argv)
{
  /* Parse command-line options. */
  auto optionsParser = CommonOptionsParser::create(argc, argv, g_ToolCategory);
  ClangTool tool(optionsParser->getCompilations(), optionsParser->getSourcePathList());

  MatchFinder finder;
  Finder my_finder;

  /* Search for all fields with an 'annotate' attribute. */
  static DeclarationMatcher const propertyMatcher =
      fieldDecl(decl().bind("id"), isExpansionInMainFile(),  hasAttr(clang::attr::Annotate));
  finder.addMatcher(propertyMatcher, &my_finder);

  tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-xc++", ArgumentInsertPosition::BEGIN));

  return tool.run(newFrontendActionFactory(&finder).get());
}

