#include "rules/rule560.hpp"
#include <clang/AST/ExprCXX.h>
#include <clang/Basic/SourceManager.h>
#include <iostream>

bool rule560::containsBoolLiteral(const clang::Expr *expr)
{
    if (!expr)
        return false;

    const clang::Expr *cleanExpr = expr->IgnoreParenImpCasts();

    if (llvm::isa<clang::CXXBoolLiteralExpr>(cleanExpr))
    {
        return true;
    }

    if (const auto *binOp = llvm::dyn_cast<clang::BinaryOperator>(cleanExpr))
    {
        if (binOp->isLogicalOp())
        {
            return containsBoolLiteral(binOp->getLHS()) ||
                   containsBoolLiteral(binOp->getRHS());
        }
    }

    return false;
}

void rule560::check(clang::IfStmt *stmt, clang::ASTContext *context)
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

    const auto *binOp = llvm::dyn_cast<clang::BinaryOperator>(cond->IgnoreParenImpCasts());
    if (binOp && binOp->isLogicalOp() && containsBoolLiteral(binOp))
    {
        unsigned lineNo = sm.getSpellingLineNumber(loc);
        std::cout << "\033[1;31m[WARNING 560]\033[0m A part of conditional expression is always true/false "
                  << "at line " << lineNo << std::endl;
    }
}