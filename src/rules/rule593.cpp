#include "rules/rule593.hpp"
#include <clang/Basic/SourceManager.h>
#include <iostream>

void rule593::check(clang::IfStmt *stmt, clang::ASTContext *context)
{
    if (!stmt || !context)
        return;

    clang::SourceManager &sm = context->getSourceManager();
    clang::SourceLocation loc = stmt->getIfLoc();

    if (sm.isInSystemHeader(loc))
    {
        return;
    }

    const clang::Expr *cond = stmt->getCond();
    if (!cond)
        return;

    const clang::Expr *cleanCond = cond->IgnoreImpCasts();

    if (const auto *binOp = llvm::dyn_cast<clang::BinaryOperator>(cleanCond))
    {
        if (binOp->getOpcode() == clang::BO_Assign)
        {
            unsigned lineNo = sm.getSpellingLineNumber(binOp->getOperatorLoc());
            std::cout << "\033[1;31m[WARNING 593]\033[0m Unparenthesized assignment in conditional expression "
                      << "at line " << lineNo << std::endl;
        }
    }
}