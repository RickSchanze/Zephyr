/**
 * @file Finder.cpp
 * @author Echo
 * @Date 24-3-12
 * @brief
 */
#include "Finder.h"

#include <filesystem>
#include <iostream>
#include <ranges>

using namespace clang;

void Finder::run(const ast_matchers::MatchFinder::MatchResult &Result)
{
    const auto *decl = Result.Nodes.getNodeAs<FieldDecl>("id");
    m_context = Result.Context;
    m_sourceman = Result.SourceManager;
    if (decl)
    {
        FoundField(decl);
    }
}
void Finder::onEndOfTranslationUnit()
{
    /* For stdout output. */
    /* raw_fd_ostream os(1, false); */
    std::error_code ec;
    if (m_classes.empty())
        return;
    const auto path = output_file_path / m_filename;
    llvm::raw_fd_ostream os(path.string(), ec);
    assert(!ec && "error opening file");
    os << "#pragma once\n";
    os << "#include \"Reflection/MetaType.h\"\n";
    for (auto &val : m_classes | std::views::values)
    {
        val.Generate(m_context, os);
    }
}

void Finder::FoundField(const FieldDecl *field)
{
    m_filename = m_sourceman->getFilename(field->getLocation()).str();
    if (m_filename.ends_with(".generated.h")) return;
    const auto file_path = std::filesystem::path(m_filename);
    m_filename = file_path.filename().string();
    m_filename.erase(m_filename.end() - 2, m_filename.end());
    m_filename.append(".generated.h");
    const auto parent_class = static_cast<const CXXRecordDecl *>(field->getParent());
    const auto parent_name = parent_class->getQualifiedNameAsString();
    if (!m_classes.contains(parent_name))
    {
        m_classes[parent_name] = ReflectedClass(parent_class);
        m_classes[parent_name].AddField(field);
    }
    else
    {
        m_classes[parent_name].AddField(field);
    }
}

void GeneratorDiagnosticConsumer::HandleDiagnostic(DiagnosticsEngine::Level DiagLevel, const Diagnostic &Info)
{
}
