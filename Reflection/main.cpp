#include "Finder.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace clang::tooling;
using namespace clang::ast_matchers;

static llvm::cl::OptionCategory g_ToolCategory("metareflect options");
static llvm::cl::opt<std::string> output_file_path("output-file-path", llvm::cl::desc("Output file path"),
                                                   llvm::cl::value_desc("path"), llvm::cl::cat(g_ToolCategory),
                                                   llvm::cl::Required);
static llvm::cl::list<std::string> include_path("include-path", llvm::cl::desc("Include path"),
                                               llvm::cl::value_desc("path"), llvm::cl::cat(g_ToolCategory),
                                               llvm::cl::Required);

void AddIncludePaths(ClangTool &tool)
{
    for (const auto &path : include_path)
    {
        std::string path_command = std::string("-I") + path;
        tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(path_command.c_str()));
    }
}

int main(int argc, const char **argv)
{
    /* Parse command-line options. */
    auto optionsParser = CommonOptionsParser::create(argc, argv, g_ToolCategory);
    ClangTool tool(optionsParser->getCompilations(), optionsParser->getSourcePathList());
    MatchFinder finder;
    Finder my_finder;

    /* Search for all fields with an 'annotate' attribute. */
    static DeclarationMatcher const propertyMatcher =
        fieldDecl(decl().bind("id"), isExpansionInMainFile(), hasAttr(clang::attr::Annotate));
    finder.addMatcher(propertyMatcher, &my_finder);

    tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-xc++", ArgumentInsertPosition::BEGIN));
    tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-std=c++20", ArgumentInsertPosition::BEGIN));
    tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-DREFLECTION", ArgumentInsertPosition::BEGIN));
    GeneratorDiagnosticConsumer consumer;
    tool.setDiagnosticConsumer(&consumer);
    AddIncludePaths(tool);
    if (std::filesystem::exists(output_file_path.c_str()))
    {
        Finder::output_file_path = output_file_path.c_str();
    }
    const int result = tool.run(newFrontendActionFactory(&finder).get());
    return result;
}
