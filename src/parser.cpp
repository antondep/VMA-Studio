#include "parser.hpp"
#include "rules/rule501.hpp"
#include "rules/rule570.hpp"
#include "rules/rule502.hpp"
#include "rules/rule606.hpp"
#include "rules/rule517.hpp"
#include "rules/rule523.hpp"
#include "rules/rule560.hpp"
#include "rules/rule593.hpp"
#include <clang/Tooling/CommonOptionsParser.h>
#include <llvm/Support/CommandLine.h>

using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory AnalyzerCategory("Custom Static Analyzer Options");

bool ASTVisitor::VisitBinaryOperator(clang::BinaryOperator *op)
{
    rule501::check(op, context_);
    rule570::check(op, context_);
    rule502::check(op, context_);
    rule606::check(op, context_);
    return true;
}

bool ASTVisitor::VisitIfStmt(clang::IfStmt *stmt)
{
    rule517::check(stmt, context_);
    rule523::check(stmt, context_);
    rule560::check(stmt, context_);
    rule593::check(stmt, context_);
    return true;
}

int main(int argc, const char **argv)
{
    auto expectedParser = CommonOptionsParser::create(argc, argv, AnalyzerCategory);
    if (!expectedParser)
    {
        llvm::errs() << expectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &optionsParser = expectedParser.get();
    ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    return tool.run(newFrontendActionFactory<AnalyzerFrontendAction>().get());
}